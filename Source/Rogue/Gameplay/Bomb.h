#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bomb.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UEventBus;
class UNiagaraComponent;
class USoundBase;
class UAudioComponent;

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

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> BombExplosionClass;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* BombThrowSound;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* BombExplosionSound;

  public:
    ABomb();
    void LaunchInDirection(const FVector& Direction) const;
    void SetWeaponDamage(int32 NewDamage) { WeaponDamage = NewDamage; }

  protected:
    void BeginPlay() override;
    void EndPlay(EEndPlayReason::Type EndPlayReason) override;

  private:
    // Gets a list of all actors in the blast radius and deals damage to them.
    void Explode();

    // Should only be called when a wall or floor is hit.
    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
               FVector NormalImpulse, const FHitResult& Hit)
    {
        Explode();
    }

    // Should only be called when an enemy is hit.
    UFUNCTION()
    void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
    {
        Explode();
    }
};
