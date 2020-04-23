// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "SynchInterHud.generated.h"


UCLASS(config = Game)
class ASynchInterHud : public AHUD
{
	GENERATED_BODY()

public:
	ASynchInterHud();

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface
};
