#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "CoreMinimal.h"
#include "PlayerHUD.generated.h"

class UImage;
class UTextBlock;
struct FTimespan;

UCLASS()
class ROGUE_API UPlayerHUD : public UUserWidget
{
    GENERATED_BODY()

    FTimespan GameTimer;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UImage* Crosshair;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* ExperienceBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TimerText;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* DamageFlash;

  public:
    virtual void NativeConstruct() override;
    void SetHealth(float CurrentHealth, float MaxHealth) { HealthBar->SetPercent(CurrentHealth / MaxHealth); }
    void SetExperience(float CurrentExp, float MaxExp) { ExperienceBar->SetPercent(CurrentExp / MaxExp); }
    void UpdateTimer();
    void PlayDamageAnimation() { PlayAnimation(DamageFlash); }
};
