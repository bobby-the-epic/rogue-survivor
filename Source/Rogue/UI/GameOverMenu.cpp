#include "GameOverMenu.h"
#include "Components/Button.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"

void UGameOverMenu::NativeConstruct()
{
    Super::NativeConstruct();
    RestartButton->OnReleased.AddDynamic(this, &UGameOverMenu::RestartGame);
    QuitButton->OnReleased.AddDynamic(this, &UGameOverMenu::QuitGame);
}
void UGameOverMenu::NativeDestruct()
{
    RestartButton->OnReleased.RemoveDynamic(this, &UGameOverMenu::RestartGame);
    Super::NativeDestruct();
}
void UGameOverMenu::RestartGame()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), GameMap);
}
void UGameOverMenu::QuitGame()
{
    UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), MainMenuMap);
}
