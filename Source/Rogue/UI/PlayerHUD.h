#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "PlayerHUD.generated.h"

class UProgressBar;
class UImage;
class UTextBlock;
struct FTimespan;

UCLASS()
class ROGUE_API UPlayerHUD : public UUserWidget
{
    GENERATED_BODY()

    FTimespan GameTimer;

    UPROPERTY(EditDefaultsOnly)
    int32 TimerMinutes = 30;

    UPROPERTY(EditDefaultsOnly)
    int32 TimerSeconds = 0;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    UImage* Crosshair;

    UPROPERTY(meta = (BindWidget))
    UProgressBar* ExperienceBar;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* TimerText;

  public:
    virtual void NativeConstruct() override;
    void SetHealth(float CurrentHealth, float MaxHealth);
    void SetExperience(float CurrentExp, float MaxExp);
    void UpdateTimer();
};
