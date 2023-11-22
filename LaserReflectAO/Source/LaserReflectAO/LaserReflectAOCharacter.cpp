// Copyright Epic Games, Inc. All Rights Reserved.

#include "LaserReflectAOCharacter.h"
#include "LaserReflectAOProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId
#include "DrawDebugHelpers.h"
#include "Math/Vector.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// ALaserReflectAOCharacter

ALaserReflectAOCharacter::ALaserReflectAOCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Initialize threshold to your desired value
	//threshold = 0.5f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(100.0f, 0.0f, 10.0f);

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

void ALaserReflectAOCharacter::Tick(float DeltaTime) {

	Super::Tick(DeltaTime);

	FHitResult OutHit;

	// Getting the location of the gun
	FVector start = FP_Gun->GetComponentLocation();

	// Get the forward vector of the camera
	FVector ForwardVector = FirstPersonCameraComponent->GetForwardVector();

	// Get the end point
	FVector end = (start + (ForwardVector * lineDistance));

	// Collision Params
	FCollisionQueryParams CollisionParams;

	// Draw Debug Line to visualize the line trace
	DrawDebugLine(GetWorld(), start, end, lineColorFromGun, false, 0.1f);

	// If something is hit, this puts it as the outhit hit result variable
	bool isHit = GetWorld()->LineTraceSingleByChannel(OutHit, start, end, ECC_Visibility, CollisionParams);

	// If the line trace does hit something, print to the screen the information / if isHit is true
	if (isHit) {
		// Is it true if the outhit had a blocking hit
		if (OutHit.bBlockingHit) {
			// Print out to the screen if that is true
			if (GEngine) {
				// Get the actor's name of whatever the line hits
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Cyan, FString::Printf(TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName()));

				// Gets the impact point's information and draws a debug line from the impact point's normal direction
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Emerald, FString::Printf(TEXT("Impact Point: %s"), *OutHit.ImpactPoint.ToString()));
				DrawDebugLine(GetWorld(), OutHit.ImpactPoint, OutHit.ImpactPoint + OutHit.ImpactNormal * lineDistance, lineColorFromImpact, false, 0.1f);

				// Gets the impact normal's information and draws a debug line from the impact point to the reflection
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Normal Point: %s"), *OutHit.ImpactNormal.ToString()));

				// Calculate the line direction
				FVector lineDir = (end - start).GetSafeNormal();

				// Calculate the surface normal
				FVector normal = OutHit.ImpactNormal.GetSafeNormal();

				// Used ChatGPT to help with this part - I struggled the absolute MOST here for days before I used it, but I realize that goes against the point of the assignment
				FVector reflection = lineDir - 2 * FVector::DotProduct(lineDir, normal) * normal; // I had a similar implementation a few hours prior
				reflection.Normalize();

				// Calculate the point for the reflection line
				FVector reflectionPoint = OutHit.ImpactPoint + reflection * reflectionOffset * lineDistance;

				DrawDebugLine(GetWorld(), OutHit.ImpactPoint, reflectionPoint, lineColorForReflect, false, 0.1f);
				
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, start - 2.0f * FVector::DotProduct(OutHit.ImpactNormal, start) * OutHit.ImpactNormal, lineColorForReflect, false, 0.1f);

				/* 
				 * Need to project the ray (the start line) onto the normal.
				 * The dot product gives a scalar value, does not give a vector.
				 * n = is the normal of a surface, ray = raycast / start line
				 * If we project the ray direction onto the normal, we get this length: dot(n, ray)
				 * dot(n, ray) is going to be negative, because they are pointing in opposite directions, like a negative distance.
				 * To convert dot(n, ray) to a vector, we can multiply the dot product by the normal.
				 * This creates a vector projection, of taking the ray vector and projected it onto the normal, to get the dot product vector.
				 * Now an offset needs to be made to flip the vector.
				 * If we take the projected vector, and multiply it by 2 and offset it, we would offset it up to the reflected point.
				 * The key is to successful project it onto the negative axis, and multiplying it by 2 gives us the displacement between the two vectors.
				 * Then we want to take the negative ray direction, and subtract that vector to get to the positive vector.
				 * The full distance would be 2 * dot(n, ray) * n
				 * And then we would want to subtract that from the ray direction: ray - 2 * dot(n, ray) * n
				 */

				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, start - 2.0f * dotVector * normal, lineColorForReflect, false, 0.1f); This doesn't work
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, start - (dotVector * 2) * normal, lineColorForReflect, false, 0.1f); This doesn't work
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, start - ((dotVector * 2.0f) * normal), lineColorForReflect, false, 0.1f); This doesn't work
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, start - (dotVector * 2) * (-1 * normal), lineColorForReflect, false, 0.1f); This doesn't work
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, end - 2.0f * dotVector * normal, lineColorForReflect, false, 0.1f); End is projecting on the negative axis
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, end - (dotVector * 2) * normal, lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, end - ((dotVector * 2.0f) * normal), lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, end - (dotVector * 2) * (-1 * normal), lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, end.GetSafeNormal() - (dotVector * 2) * (-1 * normal), lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, end - (dotVector * 2).GetSafeNormal() * (-1 * normal), lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, end - (dotVector * 2) * (-1 * normal).GetSafeNormal(), lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, (end - OutHit.ImpactPoint) - (dotVector * 2) * (-1 * normal), lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, (OutHit.ImpactPoint - end) - (dotVector * 2) * (-1 * normal), lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, (OutHit.ImpactPoint - end) - 2.0f * dotVector * normal, lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, (OutHit.ImpactPoint - end) - ((2.0f * dotVector) * normal), lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, (OutHit.ImpactPoint - end) - ((2.0f * dotVector) * normal) * -1.0f, lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, (end - OutHit.ImpactPoint) - ((2.0f * dotVector) * normal) * -1.0f, lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, ((end - OutHit.ImpactPoint) * -1.0f) - ((2.0f * dotVector) * normal), lineColorForReflect, false, 0.1f);
				//DrawDebugLine(GetWorld(), OutHit.ImpactPoint, (end - OutHit.ImpactPoint).GetSafeNormal() - ((dotVector * normal) * 2), lineColorForReflect, false, 0.1f);
				// These all give the exact same result or a minor variation (all wrong results); this list was a lot, lot longer, but I reduced its size to show at least some work and thought process
			}
		}
	}

	// Old Code for the Sensor Project
	/*
	// Get the player location
	FVector start = GetLocation();
	// Get the sensor's location
	FVector end = GetSensorLocation();

	// Calculate the direction vector from player to sensor
	FVector PlayerToSensorDir = (end - start).GetSafeNormal();

	// Draw Debug Line from the player to the sensor
	//DrawDebugLine(GetWorld(), start, start + PlayerToSensorDir * lineDistance, FColor::Cyan, false, 0.1f);

	// Draw Debug Line from player to the player's forward vector
	FVector playerForward = GetForward();
	DrawDebugLine(GetWorld(), start, start + playerForward * lineDistance, lineColor, false, 0.1f);

	float lookness = FVector::DotProduct(PlayerToSensorDir, playerForward);

	bool isLooking = lookness >= threshold;

	if (isLooking == true) {
		// Draw Debug Line from the player to the sensor
		DrawDebugLine(GetWorld(), start, start + PlayerToSensorDir * lineDistance, FColor::Green, false, 0.1f);
	}
	else {
		// Draw Debug Line from the player to the sensor
		DrawDebugLine(GetWorld(), start, start + PlayerToSensorDir * lineDistance, FColor::Red, false, 0.1f);
	}
	*/

	// Debug Text
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Cyan, "Hello World");
}

void ALaserReflectAOCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALaserReflectAOCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALaserReflectAOCharacter::OnFire);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ALaserReflectAOCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ALaserReflectAOCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALaserReflectAOCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent ->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALaserReflectAOCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALaserReflectAOCharacter::LookUpAtRate);
}

void ALaserReflectAOCharacter::OnFire()
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<ALaserReflectAOProjectile>(ProjectileClass, SpawnLocation, SpawnRotation);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				// spawn the projectile at the muzzle
				World->SpawnActor<ALaserReflectAOProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void ALaserReflectAOCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALaserReflectAOCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnFire();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ALaserReflectAOCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick

//void ALaserReflectAOCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
//{
//	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
//	{
//		if (TouchItem.bIsPressed)
//		{
//			if (GetWorld() != nullptr)
//			{
//				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
//				if (ViewportClient != nullptr)
//				{
//					FVector MoveDelta = Location - TouchItem.Location;
//					FVector2D ScreenSize;
//					ViewportClient->GetViewportSize(ScreenSize);
//					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
//					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.X * BaseTurnRate;
//						AddControllerYawInput(Value);
//					}
//					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
//					{
//						TouchItem.bMoved = true;
//						float Value = ScaledDelta.Y * BaseTurnRate;
//						AddControllerPitchInput(Value);
//					}
//					TouchItem.Location = Location;
//				}
//				TouchItem.Location = Location;
//			}
//		}
//	}
//}


// Old Code for the Sensor Project
/*
FVector ALaserReflectAOCharacter::GetLocation() {
	// Funciton to get the player's location
	//return GetActorLocation();

	return FirstPersonCameraComponent->GetComponentLocation();
}

FVector ALaserReflectAOCharacter::GetForward() {
	// Funciton to get the player's forward vector
	//return GetActorForwardVector();

	return FirstPersonCameraComponent->GetForwardVector();
}

FVector ALaserReflectAOCharacter::GetSensorLocation() {
	// Funciton to get the sensor's location
	return sensor->GetActorLocation();
}

FVector ALaserReflectAOCharacter::GetSensorForward() {
	// Funciton to get the sensor's forward vector
	return sensor->GetActorForwardVector();
}
*/

void ALaserReflectAOCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void ALaserReflectAOCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void ALaserReflectAOCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALaserReflectAOCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool ALaserReflectAOCharacter::EnableTouchscreenMovement(class UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ALaserReflectAOCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ALaserReflectAOCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		//PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ALaserReflectAOCharacter::TouchUpdate);
		return true;
	}
	
	return false;
}
