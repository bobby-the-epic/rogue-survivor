// Copyright Epic Games, Inc. All Rights Reserved.

#include "RogueGameMode.h"
#include "PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARogueGameMode::ARogueGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerCharacterBP"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}
