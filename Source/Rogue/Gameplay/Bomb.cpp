#include "Bomb.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "EventBus.h"
#include "Kismet/GameplayStatics.h"
#include "MainGameInstance.h"
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
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), BombThrowSound, GetActorLocation(),
                                          GetGameInstance<UMainGameInstance>()->GetSFXVolume(), 1.0f, 0.0f,
                                          GetGameInstance<UMainGameInstance>()->GetSoundAttenuation());
}
void ABomb::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    BombMesh->OnComponentBeginOverlap.RemoveDynamic(this, &ABomb::OnOverlap);
    BombMesh->OnComponentHit.RemoveDynamic(this, &ABomb::OnHit);
    Super::EndPlay(EndPlayReason);
}
void ABomb::Explode()
{
    TArray<AActor*> HitActors;
    BlastRadius->GetOverlappingActors(HitActors, ASkeletonWarrior::StaticClass());
    for (AActor* HitActor : HitActors)
    {
        ASkeletonWarrior* Actor = Cast<ASkeletonWarrior>(HitActor);
        if (Actor != nullptr)
        {
            Actor->TakeDamage(WeaponDamage);
        }
    }
    GetWorld()->SpawnActor<AActor>(BombExplosionClass, GetActorLocation(), FRotator::ZeroRotator);
    UGameplayStatics::PlaySoundAtLocation(GetWorld(), BombExplosionSound, GetActorLocation(),
                                          GetGameInstance<UMainGameInstance>()->GetSFXVolume(), 1.0f, 0.0f,
                                          GetGameInstance<UMainGameInstance>()->GetSoundAttenuation());
    Destroy();
}
void ABomb::LaunchInDirection(const FVector& Direction) const
{
    BombMesh->AddImpulse(Direction * 7500);
    FVector RotationDirection = Direction;
    RotationDirection.Z = 0;
    BombMesh->AddAngularImpulseInDegrees(RotationDirection * 750, NAME_None, true);
}
