// Copyright Epic Games, Inc. All Rights Reserved.

#include "RogueGameMode.h"
#include "EventBus.h"
#include "Rogue/Characters/PlayerCharacter.h"
#include "Rogue/UI/PlayerHUD.h"
#include "UObject/ConstructorHelpers.h"

ARogueGameMode::ARogueGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerCharacter_BP"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
}
