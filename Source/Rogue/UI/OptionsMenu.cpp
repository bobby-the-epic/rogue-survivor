#include "OptionsMenu.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Kismet/GameplayStatics.h"
#include "Rogue/Gameplay/EventBus.h"
#include "Rogue/Gameplay/MainGameMode.h"

void UOptionsMenu::NativeConstruct()
{
    Super::NativeConstruct();
    GameInstance = GetGameInstance<UMainGameInstance>();
    BackButton->OnReleased.AddDynamic(this, &UOptionsMenu::ToggleOptionsMenu);
    // Set a timer for next tick or else it will crash the game
    GetWorld()->GetTimerManager().SetTimerForNextTick([this] {
        MusicSlider->SetValue(GameInstance->GetMusicVolume());
        SFXSlider->SetValue(GameInstance->GetSFXVolume());
    });
    MusicSlider->OnValueChanged.AddDynamic(this, &UOptionsMenu::SetMusicVolume);
    SFXSlider->OnValueChanged.AddDynamic(this, &UOptionsMenu::SetSFXVolume);
}
void UOptionsMenu::NativeDestruct()
{
    BackButton->OnReleased.RemoveDynamic(this, &UOptionsMenu::ToggleOptionsMenu);
    MusicSlider->OnValueChanged.RemoveDynamic(this, &UOptionsMenu::SetMusicVolume);
    SFXSlider->OnValueChanged.RemoveDynamic(this, &UOptionsMenu::SetSFXVolume);
    Super::NativeDestruct();
}
void UOptionsMenu::ToggleOptionsMenu()
{
    if (IsVisible())
    {
        SetVisibility(ESlateVisibility::Hidden);
        if (UGameplayStatics::GetCurrentLevelName(GetWorld()) != TEXT("MainMenuMap"))
        {
            UEventBus* EventBus = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetEventBus();
            EventBus->OnSFXVolumeChangedDelegate.Broadcast(SFXSlider->GetValue());
        }
    }
    else
    {
        SetVisibility(ESlateVisibility::Visible);
    }
}
