// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SynchInterGameMode.h"
#include "SynchInterPawn.h"
#include "SynchInterHud.h"

ASynchInterGameMode::ASynchInterGameMode()
{
	DefaultPawnClass = ASynchInterPawn::StaticClass();
	HUDClass = ASynchInterHud::StaticClass();
}
