#pragma once

#include "Humanoid.h"
#include "SkeletonWarrior.generated.h"

class UWidgetComponent;
class UEnemyHealthBar;
class UBehaviorTree;
class AExperienceOrb;

UCLASS()
class ROGUE_API ASkeletonWarrior : public AHumanoid
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bIsPlayerDead = false;

    UPROPERTY(VisibleAnywhere)
    bool bIsSpawning = true;

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

    FTimerHandle DeathTimer;

  public:
    ASkeletonWarrior();
    bool GetIsDead() const { return bIsDead; }
    virtual void TakeDamage(int32 Damage) override;
    void Knockback();
    void Die();
    void SetWeaponDamage(int32 NewDamage) { WeaponDamage = NewDamage; }
    void SetHealth(int32 NewHealth);

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

    UFUNCTION()
    virtual void LevelUp() override;

  private:
    void SpawnExperienceOrb();

    UFUNCTION()
    void UpdateHealthBarRotation(FVector CameraLocation);

    UFUNCTION()
    void EndSpawning(UAnimMontage* Montage, bool bInterrupted);

    UFUNCTION()
    void MoveMeshUp() { GetMesh()->SetRelativeLocation(FVector(0, 0, -65.0f)); }

    UFUNCTION()
    void DestroyCorpse() { Destroy(); }

    UFUNCTION(BlueprintCallable)
    void Attack() const;

    UFUNCTION()
    void Celebrate();
};
