#include "Humanoid.h"
#include "Components/AudioComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AHumanoid::AHumanoid()
{
    AudioComponent = CreateDefaultSubobject<UAudioComponent>("AudioComponent");
    AudioComponent->SetupAttachment(RootComponent);
}
void AHumanoid::BeginPlay()
{
    Super::BeginPlay();
    DamageMaterial = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
    GetMesh()->SetMaterial(0, DamageMaterial);
}
void AHumanoid::StartEmissiveColorBlend(const FLinearColor& StartLerpColor, const FLinearColor& EndLerpColor)
{
    StartColor = StartLerpColor;
    EndColor = EndLerpColor;
    EmissiveBlendTime = 0;
    LerpValue = 0;
    DamageMaterialTimer = GetWorldTimerManager().SetTimerForNextTick(this, &AHumanoid::BlendEmissiveColor);
}
void AHumanoid::BlendEmissiveColor()
{
    LerpValue = FMath::Lerp(StartColor.R, EndColor.R, EmissiveBlendTime);
    DamageMaterial->SetVectorParameterValue("EmissiveColor", FVector4(LerpValue));
    EmissiveBlendTime += GetWorld()->GetDeltaSeconds() * 7;

    if (EndColor == DamagedColor && LerpValue >= EndColor.R)
    {
        // Starts blending back to normal color
        LerpValue = 0.0f;
        EmissiveBlendTime = 0.0f;
        GetWorldTimerManager().ClearTimer(DamageMaterialTimer);
        StartEmissiveColorBlend(DamagedColor, FLinearColor::Black);
        return;
    }
    else if (EndColor == FColor::Black && LerpValue <= EndColor.R)
    {
        // Stops the blend
        LerpValue = 0.0f;
        EmissiveBlendTime = 0.0f;
        GetWorldTimerManager().ClearTimer(DamageMaterialTimer);
        return;
    }
    DamageMaterialTimer = GetWorldTimerManager().SetTimerForNextTick(this, &AHumanoid::BlendEmissiveColor);
}
