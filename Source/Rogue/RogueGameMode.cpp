// Copyright Epic Games, Inc. All Rights Reserved.

#include "RogueGameMode.h"
#include "PlayerCharacter.h"
#include "PlayerHUD.h"
#include "UObject/ConstructorHelpers.h"

ARogueGameMode::ARogueGameMode()
{
    // set default pawn class to our Blueprinted character
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/PlayerCharacterBP"));
    static ConstructorHelpers::FClassFinder<AHUD> PlayerHUDBPClass(TEXT("/Game/Blueprints/PlayerHUD_BP"));
    if (PlayerPawnBPClass.Class != NULL)
    {
        DefaultPawnClass = PlayerPawnBPClass.Class;
    }
    if (PlayerHUDBPClass.Class != NULL)
    {
        HUDClass = PlayerHUDBPClass.Class;
    }
}
