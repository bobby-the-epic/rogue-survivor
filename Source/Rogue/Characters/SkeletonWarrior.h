#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Rogue/Gameplay/CombatInterface.h"
#include "SkeletonWarrior.generated.h"

class UWidgetComponent;
class UEnemyHealthBar;
class UBehaviorTree;
class AExperienceOrb;

UCLASS()
class ROGUE_API ASkeletonWarrior : public ACharacter, public ICombatInterface
{
    GENERATED_BODY()

    UPROPERTY()
    int32 WeaponDamage = 5;

    UPROPERTY(VisibleAnywhere)
    float CurrentHealth = 100;

    UPROPERTY(VisibleAnywhere)
    float MaxHealth = 100;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool IsDead = false;

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool IsPlayerDead = false;

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

    UPROPERTY(EditDefaultsOnly, Category = Animation)
    UAnimMontage* SpawnMontage;

    UPROPERTY(EditDefaultsOnly)
    UBehaviorTree* BehaviorTree;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AExperienceOrb> ExperienceOrbBP;

  public:
    ASkeletonWarrior();
    virtual void TakeDamage(int32 Damage) override;

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

  private:
    UFUNCTION()
    void UpdateHealthBarRotation(FVector CameraLocation);

    UFUNCTION()
    void EndSpawning(UAnimMontage* Montage, bool bInterrupted);

    UFUNCTION(BlueprintCallable)
    void Attack() const;

    UFUNCTION()
    void Celebrate();
};
