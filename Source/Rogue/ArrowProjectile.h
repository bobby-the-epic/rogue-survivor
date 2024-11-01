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
    UPROPERTY(VisibleDefaultsOnly, meta = (AllowPrivateAccess = "true"))
    UStaticMeshComponent* ArrowMesh;

  public:
    AArrowProjectile();
    void FireInDirection(const FVector& ShootDirection);

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
               FVector NormalImpulse, const FHitResult& Hit);

  protected:
    virtual void BeginPlay() override;
};
