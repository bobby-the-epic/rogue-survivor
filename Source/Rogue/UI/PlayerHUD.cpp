#include "PlayerHUD.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Timespan.h"
#include "Rogue/Gameplay/EventBus.h"
#include "Rogue/Gameplay/MainGameMode.h"

void UPlayerHUD::NativeConstruct()
{
    GameTimer = FTimespan::FromMinutes(TimerInMinutes);
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
    if (GameTimer.IsZero())
    {
        // Broadcast the game over delegate
        Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetEventBus()->OnGameOverDelegate.Broadcast();
    }
    FString TimerString = GameTimer.ToString(TEXT("%m:%s"));
    TimerString.RemoveAt(0, 1, true);
    TimerText->SetText(FText::FromString(TimerString));
}
