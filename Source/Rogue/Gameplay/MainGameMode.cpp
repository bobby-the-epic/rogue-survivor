#include "MainGameMode.h"
#include "EventBus.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Rogue/Characters/PlayerCharacter.h"
#include "Rogue/Characters/SkeletonWarrior.h"

void AMainGameMode::BeginPlay()
{
    Super::BeginPlay();

    GetWorldTimerManager().SetTimer(SpawnTimerHandle, this, &AMainGameMode::SpawnSkeleton, 3.0f, true);
    EventBus = NewObject<UEventBus>();
    EventBus->OnPlayerDeathDelegate.AddDynamic(this, &AMainGameMode::StopSpawningSkeletons);
    EventBus->OnGameOverDelegate.AddDynamic(this, &AMainGameMode::GameOver);
    Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
}
void AMainGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    EventBus->OnPlayerDeathDelegate.RemoveDynamic(this, &AMainGameMode::StopSpawningSkeletons);
    EventBus->OnGameOverDelegate.RemoveDynamic(this, &AMainGameMode::GameOver);
    GetWorldTimerManager().ClearAllTimersForObject(this);
    Super::EndPlay(EndPlayReason);
}
void AMainGameMode::SpawnSkeleton() const
{
    // Spawn a skeleton at a random location in a radius around the player
    FVector PlayerLocation = Player->GetActorLocation();
    FVector RandomPoint;
    FActorSpawnParameters SpawnParams;
    UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetWorld(), PlayerLocation, RandomPoint, 3000.0f);
    FRotator SpawnRotation = (PlayerLocation - RandomPoint).GetSafeNormal().Rotation();
    ASkeletonWarrior* Skeleton =
        GetWorld()->SpawnActor<ASkeletonWarrior>(SkeletonClass, RandomPoint, SpawnRotation, SpawnParams);
}
void AMainGameMode::StopSpawningSkeletons()
{
    GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
}
void AMainGameMode::GameOver()
{
    StopSpawningSkeletons();
    // Destroy remaining skeletons
    TArray<AActor*> RemainingSkeletons;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), SkeletonClass, RemainingSkeletons);
    for (int Counter = 0; Counter < RemainingSkeletons.Num(); Counter++)
    {
        RemainingSkeletons[Counter]->Destroy();
    }
}
