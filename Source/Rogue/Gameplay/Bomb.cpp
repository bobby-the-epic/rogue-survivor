#include "Bomb.h"
#include "CombatInterface.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EventBus.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameMode.h"
#include "NiagaraComponent.h"
#include "Rogue/Characters/SkeletonWarrior.h"

ABomb::ABomb()
{
    PrimaryActorTick.bCanEverTick = false;
    BombMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BombMesh"));
    RootComponent = BombMesh;
    BlastRadius = CreateDefaultSubobject<USphereComponent>(TEXT("BlastRadius"));
    BlastRadius->SetupAttachment(BombMesh);
    BombFuse = CreateDefaultSubobject<UNiagaraComponent>(TEXT("BombFuse"));
    BombFuse->SetupAttachment(BombMesh);
}
void ABomb::BeginPlay()
{
    Super::BeginPlay();
    EventBus = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetEventBus();
    BombMesh->OnComponentBeginOverlap.AddDynamic(this, &ABomb::OnOverlap);
    BombMesh->OnComponentHit.AddDynamic(this, &ABomb::OnHit);
}
void ABomb::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    BombMesh->OnComponentBeginOverlap.RemoveDynamic(this, &ABomb::OnOverlap);
    BombMesh->OnComponentHit.RemoveDynamic(this, &ABomb::OnHit);
    Super::EndPlay(EndPlayReason);
}
void ABomb::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                  FVector NormalImpulse, const FHitResult& Hit)
{
    Explode();
}
void ABomb::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                      int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    Explode();
}
void ABomb::Explode()
{
    TArray<AActor*> HitActors;
    BlastRadius->GetOverlappingActors(HitActors, ASkeletonWarrior::StaticClass());
    for (AActor* HitActor : HitActors)
    {
        ICombatInterface* Actor = Cast<ICombatInterface>(HitActor);
        if (Actor != nullptr)
        {
            Actor->TakeDamage(WeaponDamage);
        }
    }
    Destroy();
}
