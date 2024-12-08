#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "Rogue/Gameplay/MainGameInstance.h"
#include "Rogue/Gameplay/MainGameModeBase.h"
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

    UPROPERTY()
    UMainGameInstance* GameInstance;

  public:
    UFUNCTION(BlueprintCallable)
    void ToggleOptionsMenu();

  protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

  private:
    UFUNCTION()
    void SetMusicVolume(float Value)
    {
        GameInstance->SetMusicVolume(Value);
        Cast<AMainGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()))->SetMusicVolume(Value);
    }

    UFUNCTION()
    void SetSFXVolume(float Value) { GameInstance->SetSFXVolume(Value); }
};
