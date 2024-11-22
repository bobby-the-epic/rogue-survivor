#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "DeathMenu.generated.h"

class UButton;

UCLASS()
class ROGUE_API UDeathMenu : public UUserWidget
{
    GENERATED_BODY()

    UPROPERTY(meta = (BindWidget))
    UButton* RetryButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    UPROPERTY(Transient, meta = (BindWidgetAnim))
    UWidgetAnimation* FadeInAnimation;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> MainMenuMap;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> GameMap;

  protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

  private:
    UFUNCTION()
    void RetryGame();

    UFUNCTION()
    void QuitToMainMenu();
};
