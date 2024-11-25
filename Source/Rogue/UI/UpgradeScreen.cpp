#include "UpgradeScreen.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "EUpgradeType.h"
#include "Kismet/GameplayStatics.h"
#include "Rogue/Characters/PlayerCharacter.h"
#include "Rogue/Gameplay/EventBus.h"
#include "Rogue/Gameplay/MainGameMode.h"
#include "Upgrade.h"

void UUpgradeScreen::NativeConstruct()
{
    Super::NativeConstruct();
    // Bind delegates.
    EventBus = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetEventBus();
    EventBus->OnPlayerLevelUpDelegate.AddDynamic(this, &UUpgradeScreen::ShowUpgradeScreen);
    UpgradeButtonLeft->OnReleased.AddDynamic(this, &UUpgradeScreen::ChooseLeftUpgrade);
    UpgradeButtonCenter->OnReleased.AddDynamic(this, &UUpgradeScreen::ChooseCenterUpgrade);
    UpgradeButtonRight->OnReleased.AddDynamic(this, &UUpgradeScreen::ChooseRightUpgrade);

    // Adds the images to a TArray just to make things easier.
    UpgradeButtonImages.Push(ImageLeft);
    UpgradeButtonImages.Push(ImageCenter);
    UpgradeButtonImages.Push(ImageRight);

    for (TSubclassOf<UUpgrade> UpgradeClass : UpgradeClasses)
    {
        // Create the object and add it to the array
        UUpgrade* Upgrade = NewObject<UUpgrade>(this, UpgradeClass);
        UpgradeObjects.Push(Upgrade);
    }
}
void UUpgradeScreen::NativeDestruct()
{
    // Unbind delegates.
    EventBus->OnPlayerLevelUpDelegate.RemoveDynamic(this, &UUpgradeScreen::ShowUpgradeScreen);
    UpgradeButtonLeft->OnReleased.RemoveDynamic(this, &UUpgradeScreen::ChooseLeftUpgrade);
    UpgradeButtonCenter->OnReleased.RemoveDynamic(this, &UUpgradeScreen::ChooseCenterUpgrade);
    UpgradeButtonRight->OnReleased.RemoveDynamic(this, &UUpgradeScreen::ChooseRightUpgrade);
    Super::NativeDestruct();
}
void UUpgradeScreen::ShowUpgradeScreen()
{
    ShuffleUpgrades();
    // Shows the widget and pauses the game to let the player choose an upgrade.
    SetVisibility(ESlateVisibility::Visible);
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
    UGameplayStatics::SetGamePaused(GetWorld(), true);
}
void UUpgradeScreen::HideUpgradeScreen()
{
    SetVisibility(ESlateVisibility::Hidden);
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(false);
    UGameplayStatics::SetGamePaused(GetWorld(), false);
}
void UUpgradeScreen::ShuffleUpgrades()
{
    // Puts the textures on the buttons
    for (int Counter = 0; Counter < 3; Counter++)
    {
        int32 RandomNum = FMath::RandHelper(UpgradeObjects.Num());
        UpgradeButtonImages[Counter]->SetBrushFromTexture(UpgradeObjects[RandomNum]->GetTexture());
        PresentedUpgrades.Push(UpgradeObjects[RandomNum]);
        UpgradeObjects.RemoveAt(RandomNum);
    }
}
void UUpgradeScreen::ChooseLeftUpgrade()
{
    HideUpgradeScreen();
    ChooseUpgrade(Left);
}
void UUpgradeScreen::ChooseCenterUpgrade()
{
    HideUpgradeScreen();
    ChooseUpgrade(Center);
}
void UUpgradeScreen::ChooseRightUpgrade()
{
    HideUpgradeScreen();
    ChooseUpgrade(Right);
}
void UUpgradeScreen::ChooseUpgrade(EButtonPos ButtonPos)
{
    EUpgradeType UpgradeType = PresentedUpgrades[ButtonPos]->GetType();

    // Removes the upgrades that would be useless if chosen more than once.
    if (UpgradeType == EUpgradeType::FullAutoFire || UpgradeType == EUpgradeType::Multishot ||
        UpgradeType == EUpgradeType::BombThrow || UpgradeType == EUpgradeType::KnockbackArrows ||
        UpgradeType == EUpgradeType::AxeDefense)
    {
        PresentedUpgrades.RemoveAt(ButtonPos);
    }

    // Puts the presented upgrades back into the upgrade pool.

    // Make a separate variable because the array shrinks.
    int ArraySize = PresentedUpgrades.Num();
    for (int Counter = 0; Counter < ArraySize; Counter++)
    {
        UpgradeObjects.Push(PresentedUpgrades.Pop());
    }

    // Get the player and apply the upgrade
    APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
    Player->ApplyUpgrade(UpgradeType);
}
