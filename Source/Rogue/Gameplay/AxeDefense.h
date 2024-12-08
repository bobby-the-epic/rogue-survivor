#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AxeDefense.generated.h"

class USceneComponent;
class URotatingMovementComponent;
class UChildActorComponent;
class USphereComponent;
class USoundBase;

UCLASS()
class ROGUE_API AAxeDefense : public AActor
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere)
    int32 WeaponDamage = 5;

    UPROPERTY()
    FTimerHandle DamageTimer;

    UPROPERTY(VisibleDefaultsOnly)
    USceneComponent* SceneComponent;

    UPROPERTY(VisibleDefaultsOnly)
    UChildActorComponent* Axe1;

    UPROPERTY(VisibleDefaultsOnly)
    UChildActorComponent* Axe2;

    UPROPERTY(VisibleDefaultsOnly)
    UChildActorComponent* Axe3;

    UPROPERTY(VisibleDefaultsOnly)
    UChildActorComponent* Axe4;

    UPROPERTY(VisibleDefaultsOnly)
    URotatingMovementComponent* RotatingComponent;

    UPROPERTY(VisibleDefaultsOnly)
    USphereComponent* SphereCollider;

    UPROPERTY()
    ACharacter* Player;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* AxeChopSound;

  public:
    AAxeDefense();
    void SetWeaponDamage(int32 NewDamage) { WeaponDamage = NewDamage; }

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

  private:
    UFUNCTION()
    void DealDamage();

    UFUNCTION()
    void UpdateLocation(float DeltaSeconds, FVector OldLocation, FVector OldVelocity) { SetActorLocation(OldLocation); }
};
