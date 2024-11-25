#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "GameOverMenu.generated.h"

class UButton;

UCLASS()
class ROGUE_API UGameOverMenu : public UUserWidget
{
    GENERATED_BODY()

    UPROPERTY(meta = (BindWidget))
    UButton* RestartButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> MainMenuMap;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> GameMap;

  protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

  private:
    UFUNCTION()
    void RestartGame();
    UFUNCTION()
    void QuitGame();
};
