#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "OptionsMenu.generated.h"

class UButton;
class USlider;

UCLASS()
class ROGUE_API UOptionsMenu : public UUserWidget
{
    GENERATED_BODY()

    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

    UPROPERTY(meta = (BindWidget))
    USlider* MusicSlider;

    UPROPERTY(meta = (BindWidget))
    USlider* SFXSlider;

  public:
    UFUNCTION(BlueprintCallable)
    void ToggleOptionsMenu();

  protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;
};
