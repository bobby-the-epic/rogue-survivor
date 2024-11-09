#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "EnemyHealthBar.generated.h"

class UProgressBar;

UCLASS()
class ROGUE_API UEnemyHealthBar : public UUserWidget
{
    GENERATED_BODY()

  public:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;
};
