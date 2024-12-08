#pragma once

#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Humanoid.generated.h"

class UEventBus;
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
    USoundBase* AttackSound;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AActor> LevelUpEffectClass;

  public:
    AHumanoid();
    virtual void TakeDamage(int32 Damage);

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
    virtual void LevelUp()
    {
        GetWorld()->SpawnActor<AActor>(LevelUpEffectClass, GetActorLocation(), FRotator::ZeroRotator);
    }
    void StartEmissiveColorBlend(const FLinearColor& StartLerpColor, const FLinearColor& EndLerpColor);

    UFUNCTION()
    void BlendEmissiveColor();

  private:
    UFUNCTION()
    void SetAudioVolume(const float NewVolume) { AudioComponent->SetVolumeMultiplier(NewVolume); }
};
