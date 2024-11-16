#include "PlayerHUD.h"
#include "Components/ProgressBar.h"

void UPlayerHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
    HealthBar->SetPercent(CurrentHealth / MaxHealth);
}
