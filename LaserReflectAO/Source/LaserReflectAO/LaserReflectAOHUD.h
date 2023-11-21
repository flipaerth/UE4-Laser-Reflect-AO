// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LaserReflectAOHUD.generated.h"

UCLASS()
class ALaserReflectAOHUD : public AHUD
{
	GENERATED_BODY()

public:
	ALaserReflectAOHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

