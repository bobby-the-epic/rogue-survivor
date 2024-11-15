#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExperienceOrb.generated.h"

class UNiagaraComponent;
class USphereComponent;
class UEventBus;

UCLASS()
class ROGUE_API AExperienceOrb : public AActor
{
    GENERATED_BODY()

    UPROPERTY()
    UEventBus* EventBus;

    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* SceneTransform;

    UPROPERTY(VisibleDefaultsOnly)
    UNiagaraComponent* ParticleSystem;

    UPROPERTY(VisibleDefaultsOnly)
    USphereComponent* SphereCollider;

  public:
    AExperienceOrb();

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

  private:
    UFUNCTION()
    void CollectExperience(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void Magnetize(AActor* Player);
};
