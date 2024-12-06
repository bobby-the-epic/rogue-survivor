#include "AxeDefense.h"
#include "Components/ChildActorComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Rogue/Characters/SkeletonWarrior.h"

AAxeDefense::AAxeDefense()
{
    PrimaryActorTick.bCanEverTick = false;
    SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
    RootComponent = SceneComponent;
    Axe1 = CreateDefaultSubobject<UChildActorComponent>("Axe1");
    Axe1->SetupAttachment(SceneComponent);
    Axe2 = CreateDefaultSubobject<UChildActorComponent>("Axe2");
    Axe2->SetupAttachment(SceneComponent);
    Axe3 = CreateDefaultSubobject<UChildActorComponent>("Axe3");
    Axe3->SetupAttachment(SceneComponent);
    Axe4 = CreateDefaultSubobject<UChildActorComponent>("Axe4");
    Axe4->SetupAttachment(SceneComponent);
    RotatingComponent = CreateDefaultSubobject<URotatingMovementComponent>("RotatingComponent");
    SphereCollider = CreateDefaultSubobject<USphereComponent>("SphereCollider");
    SphereCollider->SetupAttachment(SceneComponent);
}
void AAxeDefense::BeginPlay()
{
    Super::BeginPlay();
    GetWorldTimerManager().SetTimer(DamageTimer, this, &AAxeDefense::DealDamage, 1.5f, true);
    Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    Player->OnCharacterMovementUpdated.AddDynamic(this, &AAxeDefense::UpdateLocation);
}
void AAxeDefense::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    GetWorldTimerManager().ClearTimer(DamageTimer);
    Player->OnCharacterMovementUpdated.RemoveDynamic(this, &AAxeDefense::UpdateLocation);
    Super::EndPlay(EndPlayReason);
}
void AAxeDefense::DealDamage()
{
    // Deals damage to overlapping skeleton warriors

    TArray<AActor*> HitActors;
    SphereCollider->GetOverlappingActors(HitActors, ASkeletonWarrior::StaticClass());
    if (HitActors.Num() > 0)
    {
        for (int Counter = 0; Counter < HitActors.Num(); Counter++)
        {
            ASkeletonWarrior* Skeleton = Cast<ASkeletonWarrior>(HitActors[Counter]);
            Skeleton->TakeDamage(WeaponDamage);
        }
    }
}
