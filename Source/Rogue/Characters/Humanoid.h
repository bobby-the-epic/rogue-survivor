#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Humanoid.generated.h"

class UEventBus;
class UAudioComponent;
class USoundBase;
class UMaterialInstanceDynamic;

UCLASS(Abstract)
class AHumanoid : public ACharacter
{
    GENERATED_BODY()

  protected:
    float LerpValue = 0.0f;
    float EmissiveBlendTime = 0.0f;
    FTimerHandle DamageMaterialTimer;
    FLinearColor DamagedColor = FLinearColor(FVector4(0.2));
    FLinearColor StartColor, EndColor;

    UPROPERTY(VisibleDefaultsOnly)
    int32 CurrentHealth = 100;

    UPROPERTY(VisibleDefaultsOnly)
    int32 MaxHealth = 100;

    UPROPERTY(EditDefaultsOnly)
    int32 WeaponDamage = 10;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bIsDead = false;

    UPROPERTY()
    UEventBus* EventBus;

    UPROPERTY()
    UMaterialInstanceDynamic* DamageMaterial;

    UPROPERTY(VisibleDefaultsOnly)
    UAudioComponent* AudioComponent;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* DeathSound;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* HitSound;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* FootStepSound;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* AttackingSound;

  public:
    AHumanoid();
    virtual void TakeDamage(int32 Damage) { StartEmissiveColorBlend(FLinearColor::Black, DamagedColor); }

  protected:
    virtual void BeginPlay() override;
    void StartEmissiveColorBlend(const FLinearColor& StartLerpColor, const FLinearColor& EndLerpColor);

    UFUNCTION()
    void BlendEmissiveColor();
};
