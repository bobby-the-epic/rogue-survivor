#include "AxeDefense.h"
#include "CombatInterface.h"
#include "Components/ChildActorComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
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
}
void AAxeDefense::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    GetWorldTimerManager().ClearTimer(DamageTimer);
    Super::EndPlay(EndPlayReason);
}
void AAxeDefense::DealDamage()
{
    // Deals damage to overlapping actors

    TArray<AActor*> HitActors;
    SphereCollider->GetOverlappingActors(HitActors, ASkeletonWarrior::StaticClass());
    if (HitActors.Num() > 0)
    {
        for (int Counter = 0; Counter < HitActors.Num(); Counter++)
        {
            ICombatInterface* DamagableActor = Cast<ICombatInterface>(HitActors[Counter]);
            DamagableActor->TakeDamage(WeaponDamage);
        }
    }
}
