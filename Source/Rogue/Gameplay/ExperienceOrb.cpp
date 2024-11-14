#include "Rogue/Gameplay/ExperienceOrb.h"
#include "Components/SphereComponent.h"
#include "EventBus.h"
#include "NiagaraComponent.h"
#include "Rogue/Characters/PlayerCharacter.h"

AExperienceOrb::AExperienceOrb()
{
    PrimaryActorTick.bCanEverTick = false;
    SceneTransform = CreateDefaultSubobject<USceneComponent>(TEXT("SceneTransform"));
    RootComponent = SceneTransform;
    ParticleSystem = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ParticleSystem"));
    ParticleSystem->SetupAttachment(SceneTransform);
    SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
    SphereCollider->SetupAttachment(SceneTransform);
}
void AExperienceOrb::BeginPlay()
{
    Super::BeginPlay();

    SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &AExperienceOrb::CollectExperience);
}
void AExperienceOrb::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    SphereCollider->OnComponentBeginOverlap.RemoveDynamic(this, &AExperienceOrb::CollectExperience);

    Super::EndPlay(EndPlayReason);
}
void AExperienceOrb::CollectExperience(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                       UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                       const FHitResult& SweepResult)
{
    // The player should be the only actor that can overlap, but
    // check the cast just in case.
    if (Cast<APlayerCharacter>(OtherActor))
    {
        UEventBus::Get()->OnCollectiblePickupDelegate.Broadcast();
    }
    Destroy();
}
