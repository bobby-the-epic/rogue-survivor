#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ArrowProjectile.generated.h"

UCLASS()
class ROGUE_API AArrowProjectile : public AActor
{
    GENERATED_BODY()

  public:
    UPROPERTY(VisibleDefaultsOnly)
    UProjectileMovementComponent* ProjectileMovementComponent;

  private:
    UPROPERTY(VisibleAnywhere)
    int32 WeaponDamage = 5;

    UPROPERTY(VisibleAnywhere)
    bool bHasKnockback = false;

    UPROPERTY(VisibleDefaultsOnly)
    UStaticMeshComponent* ArrowMesh;

  public:
    AArrowProjectile();
    void SetKnockback(bool KnockbackValue) { bHasKnockback = KnockbackValue; }

    FORCEINLINE void SetWeaponDamage(int32 Damage) { WeaponDamage = Damage; }

    UFUNCTION(BlueprintCallable)
    void FireInDirection(const FVector& ShootDirection);

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
               FVector NormalImpulse, const FHitResult& Hit);

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
