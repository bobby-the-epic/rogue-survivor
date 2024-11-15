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
    UPROPERTY(VisibleAnywhere)
    UProjectileMovementComponent* ProjectileMovementComponent;

  private:
    UPROPERTY(VisibleDefaultsOnly)
    UStaticMeshComponent* ArrowMesh;

    UPROPERTY(VisibleAnywhere)
    int32 WeaponDamage = 5;

  public:
    AArrowProjectile();

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
