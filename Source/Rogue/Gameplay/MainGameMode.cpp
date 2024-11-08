#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Rogue/Characters/PlayerCharacter.h"
#include "Rogue/Characters/SkeletonWarrior.h"
#include "UObject/ConstructorHelpers.h"

void AMainGameMode::BeginPlay()
{
    Super::BeginPlay();

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, 3.0f, true);
}
void AMainGameMode::SpawnSkeleton() const
{
}
