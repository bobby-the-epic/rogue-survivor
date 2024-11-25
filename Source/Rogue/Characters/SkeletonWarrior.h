#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Rogue/Gameplay/CombatInterface.h"
#include "SkeletonWarrior.generated.h"

class UWidgetComponent;
class UEnemyHealthBar;
class UBehaviorTree;
class AExperienceOrb;
class UEventBus;

UCLASS()
class ROGUE_API ASkeletonWarrior : public ACharacter, public ICombatInterface
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    int32 WeaponDamage = 5;

    UPROPERTY(EditDefaultsOnly)
    int32 CurrentHealth = 30;

    UPROPERTY(EditDefaultsOnly)
    int32 MaxHealth = 30;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool IsDead = false;

    UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool IsPlayerDead = false;

    UPROPERTY(VisibleAnywhere)
    bool IsSpawning = true;

    UPROPERTY()
    UEventBus* EventBus;

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
    virtual void Knockback() override;
    void Die();

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

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
