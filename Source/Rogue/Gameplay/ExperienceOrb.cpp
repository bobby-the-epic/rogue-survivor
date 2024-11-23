#include "Rogue/Gameplay/ExperienceOrb.h"
#include "Components/SphereComponent.h"
#include "EventBus.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Rogue/Characters/PlayerCharacter.h"
#include "Rogue/Gameplay/MainGameMode.h"

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

    EventBus = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetEventBus();
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
    APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
    if (!Player)
    {
        return;
    }
    if (OtherComp->ComponentHasTag(TEXT("Collector")))
    {
        FTimerHandle TimerHandle;
        FTimerDelegate TimerDelegate;
        TimerDelegate.BindUFunction(this, TEXT("Magnetize"), Player);
        GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.02f, true);
    }
    else
    {
        EventBus->OnCollectiblePickupDelegate.Broadcast();
        Destroy();
    }
}
void AExperienceOrb::Magnetize(AActor* Player)
{
    // Moves the orb towards the player

    FVector TargetLocation = Player->GetActorLocation();
    TargetLocation.Z -= 75;
    FVector NewLocation =
        FMath::VInterpConstantTo(GetActorLocation(), TargetLocation, GetWorld()->GetDeltaSeconds(), 300.0f);
    SetActorLocation(NewLocation);
}
