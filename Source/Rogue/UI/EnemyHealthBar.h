#pragma once

#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "CoreMinimal.h"
#include "EnemyHealthBar.generated.h"

UCLASS()
class ROGUE_API UEnemyHealthBar : public UUserWidget
{
    GENERATED_BODY()

    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

  public:
    void SetHealth(float CurrentHealth, float MaxHealth) { HealthBar->SetPercent(CurrentHealth / MaxHealth); }
};
