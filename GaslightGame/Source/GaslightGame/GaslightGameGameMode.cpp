// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GaslightGame.h"
#include "GaslightGameGameMode.h"
#include "GaslightGamePlayerController.h"
#include "GaslightGameCharacter.h"

AGaslightGameGameMode::AGaslightGameGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = AGaslightGamePlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/TopDown/Blueprints/TopDownCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}