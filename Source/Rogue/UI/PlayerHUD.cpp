#include "PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Timespan.h"
#include "Rogue/Gameplay/EventBus.h"
#include "Rogue/Gameplay/MainGameMode.h"

void UPlayerHUD::NativeConstruct()
{
    Super::NativeConstruct();
    GameTimer = FTimespan(0, TimerMinutes, TimerSeconds);
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
    // Subtract a second from the timer
    FTimespan Second = FTimespan::FromSeconds(1.0);
    GameTimer -= Second;

    // Convert the timer to an FString
    FString TimerString;
    if (GameTimer.IsZero())
    {
        // Broadcast the game over delegate
        Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetEventBus()->OnGameOverDelegate.Broadcast();
    }
    // Format the timer string based on the time remaining
    if (GameTimer.GetMinutes() == 0)
    {
        TimerString = GameTimer.ToString(TEXT(":%s"));
    }
    else
    {
        TimerString = GameTimer.ToString(TEXT("%m:%s"));
    }
    // Remove the +/- from the timer
    TimerString.RemoveAt(0, 1, true);
    TimerText->SetText(FText::FromString(TimerString));
}
