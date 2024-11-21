#include "PauseMenu.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Rogue/Characters/PlayerCharacter.h"

void UPauseMenu::NativeConstruct()
{
    Super::NativeConstruct();
    // Bind delegates

    APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayer()->GetCharacter());
    ResumeButton->OnReleased.AddDynamic(Player, &APlayerCharacter::TogglePauseMenu);
    BackButton->OnReleased.AddDynamic(this, &UPauseMenu::ToggleQuitMenu);
    QuitButton->OnReleased.AddDynamic(this, &UPauseMenu::ToggleQuitMenu);
    QuitToMainMenuButton->OnReleased.AddDynamic(this, &UPauseMenu::QuitToMainMenu);
    QuitGameButton->OnReleased.AddDynamic(this, &UPauseMenu::QuitGame);
}
void UPauseMenu::NativeDestruct()
{
    // Unbind delegates

    APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwningPlayer()->GetCharacter());
    ResumeButton->OnReleased.RemoveDynamic(Player, &APlayerCharacter::TogglePauseMenu);
    BackButton->OnReleased.RemoveDynamic(this, &UPauseMenu::ToggleQuitMenu);
    QuitButton->OnReleased.RemoveDynamic(this, &UPauseMenu::ToggleQuitMenu);
    QuitToMainMenuButton->OnReleased.RemoveDynamic(this, &UPauseMenu::QuitToMainMenu);
    QuitGameButton->OnReleased.RemoveDynamic(this, &UPauseMenu::QuitGame);

    Super::NativeDestruct();
}
void UPauseMenu::ResumeGame()
{
    SetVisibility(ESlateVisibility::Hidden);
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
    UGameplayStatics::SetGamePaused(GetWorld(), false);
}
void UPauseMenu::ToggleQuitMenu()
{
    if (QuitMenuBorder->IsVisible())
    {
        QuitMenuBorder->SetVisibility(ESlateVisibility::Hidden);
        BackButton->SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        QuitMenuBorder->SetVisibility(ESlateVisibility::Visible);
        BackButton->SetVisibility(ESlateVisibility::Visible);
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