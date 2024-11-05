#pragma once

#include "CombatInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkeletonWarrior.generated.h"

class UWidgetComponent;
class UEnemyHealthBar;

UCLASS()
class ROGUE_API ASkeletonWarrior : public ACharacter, public ICombatInterface
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere)
    float CurrentHealth = 100;

    UPROPERTY(VisibleAnywhere)
    float MaxHealth = 100;

    UPROPERTY(VisibleDefaultsOnly)
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleDefaultsOnly)
    UStaticMeshComponent* ShieldMesh;

    UPROPERTY(VisibleDefaultsOnly)
    UCapsuleComponent* WeaponCollider;

    UPROPERTY(VisibleDefaultsOnly)
    UWidgetComponent* HealthBarWidgetComponent;

    UPROPERTY()
    UEnemyHealthBar* HealthBarWidget;

  public:
    ASkeletonWarrior();
    virtual void TakeDamage(int32 Damage) override;

  protected:
    virtual void BeginPlay() override;
    virtual void BeginDestroy() override;

  private:
    UFUNCTION()
    void UpdateHealthBarRotation(FVector CameraLocation);
};
