#include "MainGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "Rogue/Characters/SkeletonWarrior.h"

void AMainGameMode::BeginPlay()
{
    Super::BeginPlay();

    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(TimerHandle, this, &AMainGameMode::SpawnSkeleton, 3.0f, true);
}
void AMainGameMode::SpawnSkeleton() const
{
    // Spawn a skeleton at a random location in a radius around the player
    FVector PlayerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation();
    FVector RandomPoint;
    FActorSpawnParameters SpawnParams;
    UNavigationSystemV1::K2_GetRandomReachablePointInRadius(GetWorld(), PlayerLocation, RandomPoint, 3000.0f);
    FRotator SpawnRotation = (PlayerLocation - RandomPoint).GetSafeNormal().Rotation();
    ASkeletonWarrior* Skeleton =
        GetWorld()->SpawnActor<ASkeletonWarrior>(SkeletonClass, RandomPoint, SpawnRotation, SpawnParams);
}
