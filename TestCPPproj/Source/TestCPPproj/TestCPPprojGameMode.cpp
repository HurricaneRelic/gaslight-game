// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "TestCPPproj.h"
#include "TestCPPprojGameMode.h"
#include "TestCPPprojCharacter.h"

ATestCPPprojGameMode::ATestCPPprojGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
