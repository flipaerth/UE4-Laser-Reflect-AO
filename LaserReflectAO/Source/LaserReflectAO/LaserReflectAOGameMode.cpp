// Copyright Epic Games, Inc. All Rights Reserved.

#include "LaserReflectAOGameMode.h"
#include "LaserReflectAOHUD.h"
#include "LaserReflectAOCharacter.h"
#include "UObject/ConstructorHelpers.h"

ALaserReflectAOGameMode::ALaserReflectAOGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ALaserReflectAOHUD::StaticClass();
}
