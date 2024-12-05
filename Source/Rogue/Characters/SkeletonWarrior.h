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
class UMaterialInstanceDynamic;

UCLASS()
class ROGUE_API ASkeletonWarrior : public ACharacter, public ICombatInterface
{
    GENERATED_BODY()

    float LerpValue = 0.0f;
    float ElapsedTime = 0.0f;
    FTimerHandle DamageMaterialTimer;
    FLinearColor DamagedColor = FLinearColor(FVector4(0.2));
    FLinearColor StartColor, EndColor;

    UPROPERTY(VisibleAnywhere)
    int32 WeaponDamage = 5;

    UPROPERTY(VisibleAnywhere)
    int32 CurrentHealth = 30;

    UPROPERTY(VisibleAnywhere)
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

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> LevelUpEffectClass;

    UPROPERTY()
    UMaterialInstanceDynamic* DamageMaterial;

  public:
    ASkeletonWarrior();
    bool GetIsDead() const { return IsDead; }
    virtual void TakeDamage(int32 Damage) override;
    virtual void Knockback() override;
    void Die();
    void SetWeaponDamage(int32 NewDamage) { WeaponDamage = NewDamage; }
    void SetHealth(int32 NewHealth);
    void SpawnLevelUpEffect()
    {
        GetWorld()->SpawnActor<AActor>(LevelUpEffectClass, GetActorLocation(), FRotator::ZeroRotator);
    }

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

  private:
    void SpawnExperienceOrb();
    void StartEmissiveColorBlend(const FLinearColor& StartLerpColor, const FLinearColor& EndLerpColor);

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

    UFUNCTION()
    void BlendEmissiveColor();
};
