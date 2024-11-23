#pragma once

#include "Blueprint/UserWidget.h"
#include "CoreMinimal.h"
#include "UpgradeScreen.generated.h"

class UButton;
class UImage;
class UTexture2D;
class UEventBus;
class UUpgrade;

UCLASS()
class ROGUE_API UUpgradeScreen : public UUserWidget
{
    GENERATED_BODY()

    UPROPERTY()
    UEventBus* EventBus;

    UPROPERTY(meta = (BindWidget))
    UButton* UpgradeButtonLeft;

    UPROPERTY(meta = (BindWidget))
    UButton* UpgradeButtonCenter;

    UPROPERTY(meta = (BindWidget))
    UButton* UpgradeButtonRight;

    UPROPERTY(meta = (BindWidget))
    UImage* ImageLeft;

    UPROPERTY(meta = (BindWidget))
    UImage* ImageCenter;

    UPROPERTY(meta = (BindWidget))
    UImage* ImageRight;

    // The three images where the textures will appear.
    UPROPERTY()
    TArray<UImage*> UpgradeButtonImages;

    // Blueprint class array of all of the upgrades blueprints.
    UPROPERTY(EditDefaultsOnly)
    TArray<TSubclassOf<UUpgrade>> UpgradeClasses;

    // Array for the actual upgrade blueprint objects
    // for accessing the textures and enums.
    // AKA the upgrade pool.
    UPROPERTY()
    TArray<UUpgrade*> UpgradeObjects;

    // Upgrades that appear in the menu for the player to choose.
    UPROPERTY()
    TArray<UUpgrade*> PresentedUpgrades;

    // Enum to make code easier to read.
    enum EButtonPos : uint8
    {
        Left,
        Center,
        Right
    };

  protected:
    void NativeConstruct() override;
    void NativeDestruct() override;

  private:
    void HideUpgradeScreen();
    void ShuffleUpgrades();
    void ChooseUpgrade(EButtonPos ButtonPos);

    UFUNCTION(Exec)
    void ShowUpgradeScreen();

    UFUNCTION()
    void ChooseLeftUpgrade();

    UFUNCTION()
    void ChooseCenterUpgrade();

    UFUNCTION()
    void ChooseRightUpgrade();
};
