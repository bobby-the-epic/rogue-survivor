#include "DeathMenu.h"
#include "Animation/WidgetAnimation.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UDeathMenu::NativeConstruct()
{
    Super::NativeConstruct();
    RetryButton->OnReleased.AddDynamic(this, &UDeathMenu::RetryGame);
    QuitButton->OnReleased.AddDynamic(this, &UDeathMenu::QuitToMainMenu);
    PlayAnimation(FadeInAnimation);
}
void UDeathMenu::NativeDestruct()
{
    Super::NativeDestruct();
    RetryButton->OnReleased.RemoveDynamic(this, &UDeathMenu::RetryGame);
    QuitButton->OnReleased.RemoveDynamic(this, &UDeathMenu::QuitToMainMenu);
}
void UDeathMenu::RetryGame()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameMap);
}
void UDeathMenu::QuitToMainMenu()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenuMap);
}
