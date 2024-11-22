#include "OptionsMenu.h"
#include "Components/Button.h"
#include "Components/Slider.h"

void UOptionsMenu::NativeConstruct()
{
    Super::NativeConstruct();
    BackButton->OnReleased.AddDynamic(this, &UOptionsMenu::ToggleOptionsMenu);
}
void UOptionsMenu::NativeDestruct()
{
    BackButton->OnReleased.RemoveDynamic(this, &UOptionsMenu::ToggleOptionsMenu);
    Super::NativeDestruct();
}
void UOptionsMenu::ToggleOptionsMenu()
{
    if (IsVisible())
    {
        SetVisibility(ESlateVisibility::Hidden);
    }
    else
    {
        SetVisibility(ESlateVisibility::Visible);
    }
}
