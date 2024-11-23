#include "PlayerHUD.h"
#include "Components/ProgressBar.h"

void UPlayerHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
    HealthBar->SetPercent(CurrentHealth / MaxHealth);
}
void UPlayerHUD::SetExperience(float CurrentExp, float MaxExp)
{
    ExperienceBar->SetPercent(CurrentExp / MaxExp);
}
