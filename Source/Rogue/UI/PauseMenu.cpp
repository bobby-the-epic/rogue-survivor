#include "PauseMenu.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "OptionsMenu.h"
#include "Rogue/Characters/PlayerCharacter.h"
#include "UpgradeScreen.h"

void UPauseMenu::NativeConstruct()
{
    Super::NativeConstruct();
    // Bind delegates

    APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayer()->GetCharacter());
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    UpgradeScreen = Player->GetUpgradeScreen();
    OptionsMenu = CreateWidget<UOptionsMenu>(PlayerController, OptionsMenuClass);
    OptionsMenu->AddToViewport(1);
    ResumeButton->OnReleased.AddDynamic(this, &UPauseMenu::TogglePauseMenu);
    OptionsButton->OnReleased.AddDynamic(this, &UPauseMenu::ToggleOptionsMenu);
    BackButton->OnReleased.AddDynamic(this, &UPauseMenu::ToggleQuitMenu);
    QuitButton->OnReleased.AddDynamic(this, &UPauseMenu::ToggleQuitMenu);
    QuitToMainMenuButton->OnReleased.AddDynamic(this, &UPauseMenu::QuitToMainMenu);
    QuitGameButton->OnReleased.AddDynamic(this, &UPauseMenu::QuitGame);
}
void UPauseMenu::NativeDestruct()
{
    // Unbind delegates

    ResumeButton->OnReleased.RemoveDynamic(this, &UPauseMenu::TogglePauseMenu);
    OptionsButton->OnReleased.RemoveDynamic(this, &UPauseMenu::ToggleOptionsMenu);
    BackButton->OnReleased.RemoveDynamic(this, &UPauseMenu::ToggleQuitMenu);
    QuitButton->OnReleased.RemoveDynamic(this, &UPauseMenu::ToggleQuitMenu);
    QuitToMainMenuButton->OnReleased.RemoveDynamic(this, &UPauseMenu::QuitToMainMenu);
    QuitGameButton->OnReleased.RemoveDynamic(this, &UPauseMenu::QuitGame);
    OptionsMenu->RemoveFromParent();
    OptionsMenu = nullptr;

    Super::NativeDestruct();
}
void UPauseMenu::TogglePauseMenu()
{
    if (IsVisible())
    {
        if (!UpgradeScreen->IsVisible())
        {
            UGameplayStatics::SetGamePaused(GetWorld(), false);
            UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
        }
        if (OptionsMenu->IsVisible())
        {
            OptionsMenu->SetVisibility(ESlateVisibility::Hidden);
        }
        SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        UGameplayStatics::SetGamePaused(GetWorld(), true);
        SetVisibility(ESlateVisibility::Visible);
        UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
    }
}
void UPauseMenu::ToggleQuitMenu()
{
    if (QuitButtonCanvas->IsVisible())
    {
        QuitButtonCanvas->SetVisibility(ESlateVisibility::Hidden);
        BackButton->SetVisibility(ESlateVisibility::Hidden);
        ButtonCanvas->SetVisibility(ESlateVisibility::Visible);
    }
    else
    {
        QuitButtonCanvas->SetVisibility(ESlateVisibility::Visible);
        BackButton->SetVisibility(ESlateVisibility::Visible);
        ButtonCanvas->SetVisibility(ESlateVisibility::Hidden);
    }
}
void UPauseMenu::QuitToMainMenu()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenuMap);
}
void UPauseMenu::QuitGame()
{
    UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}
void UPauseMenu::ToggleOptionsMenu()
{
    OptionsMenu->ToggleOptionsMenu();
}
