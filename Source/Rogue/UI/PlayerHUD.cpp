#include "PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Misc/Timespan.h"

void UPlayerHUD::NativeConstruct()
{
    GameTimer = FTimespan::FromMinutes(30.0);
}
void UPlayerHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
    HealthBar->SetPercent(CurrentHealth / MaxHealth);
}
void UPlayerHUD::SetExperience(float CurrentExp, float MaxExp)
{
    ExperienceBar->SetPercent(CurrentExp / MaxExp);
}
void UPlayerHUD::UpdateTimer()
{
    FTimespan Second = FTimespan::FromSeconds(1.0);
    GameTimer -= Second;
    FString TimerString = GameTimer.ToString(TEXT("%m:%s"));
    TimerString.RemoveAt(0, 1, true);
    TimerText->SetText(FText::FromString(TimerString));
}
