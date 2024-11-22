#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "PauseMenu.generated.h"

class UButton;
class UBorder;
class UCanvasPanel;
class UUpgradeScreen;
class UOptionsMenu;

UCLASS()
class ROGUE_API UPauseMenu : public UUserWidget
{
    GENERATED_BODY()

    UPROPERTY(meta = (BindWidget))
    UButton* ResumeButton;

    UPROPERTY(meta = (BindWidget))
    UButton* OptionsButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitToMainMenuButton;

    UPROPERTY(meta = (BindWidget))
    UButton* QuitGameButton;

    UPROPERTY(meta = (BindWidget))
    UButton* BackButton;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* ButtonCanvas;

    UPROPERTY(meta = (BindWidget))
    UCanvasPanel* QuitButtonCanvas;

    UPROPERTY(EditDefaultsOnly)
    TSoftObjectPtr<UWorld> MainMenuMap;

    UPROPERTY()
    UUpgradeScreen* UpgradeScreen;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UOptionsMenu> OptionsMenuClass;

    UPROPERTY()
    UOptionsMenu* OptionsMenu;

  public:
    UFUNCTION(Exec)
    void TogglePauseMenu();

  protected:
    virtual void NativeConstruct() override;
    virtual void NativeDestruct() override;

  private:
    UFUNCTION()
    void ToggleQuitMenu();

    UFUNCTION()
    void QuitToMainMenu();

    UFUNCTION()
    void QuitGame();

    UFUNCTION()
    void ToggleOptionsMenu();
};
