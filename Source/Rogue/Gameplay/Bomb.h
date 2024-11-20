#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UEventBus;
class UNiagaraComponent;

UCLASS()
class ROGUE_API ABomb : public AActor
{
    GENERATED_BODY()

    UPROPERTY()
    UEventBus* EventBus;

    UPROPERTY(VisibleAnywhere)
    int32 WeaponDamage = 5;

    UPROPERTY(VisibleDefaultsOnly)
    UStaticMeshComponent* BombMesh;

    UPROPERTY(VisibleDefaultsOnly)
    USphereComponent* BlastRadius;

    UPROPERTY(VisibleDefaultsOnly)
    UNiagaraComponent* BombFuse;

  public:
    ABomb();
    FORCEINLINE void LaunchInDirection(const FVector& Direction) const { BombMesh->AddImpulse(Direction * 7500); }

  protected:
    void BeginPlay() override;
    void EndPlay(EEndPlayReason::Type EndPlayReason) override;

  private:
    // Gets a list of all actors in the blast radius and deals damage to them.
    void Explode();

    // Should only be called when a wall or floor is hit.
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
               FVector NormalImpulse, const FHitResult& Hit);

    // Should only be called when an enemy is hit.
    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
