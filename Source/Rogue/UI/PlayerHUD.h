#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "PlayerHUD.generated.h"

class UProgressBar;
class UImage;

UCLASS()
class ROGUE_API UPlayerHUD : public UUserWidget
{
    GENERATED_BODY()

  public:
    UPROPERTY(meta = (BindWidget))
    UProgressBar* HealthBar;

  private:
    UPROPERTY(meta = (BindWidget))
    UImage* Crosshair;
};
