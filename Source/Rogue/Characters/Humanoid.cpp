#include "Humanoid.h"
#include "Components/AudioComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Rogue/Gameplay/EventBus.h"
#include "Rogue/Gameplay/MainGameInstance.h"
#include "Rogue/Gameplay/MainGameMode.h"

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
    AudioComponent->SetVolumeMultiplier(GetGameInstance<UMainGameInstance>()->GetSFXVolume());
    EventBus = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetEventBus();
    EventBus->OnSFXVolumeChangedDelegate.AddDynamic(this, &AHumanoid::SetAudioVolume);
}
void AHumanoid::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    EventBus->OnSFXVolumeChangedDelegate.RemoveDynamic(this, &AHumanoid::SetAudioVolume);
    Super::EndPlay(EndPlayReason);
}
void AHumanoid::TakeDamage(int32 Damage)
{
    if (!bIsDead)
    {
        StartEmissiveColorBlend(FLinearColor::Black, DamagedColor);
        AudioComponent->SetSound(HitSound);
        AudioComponent->Play();
    }
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
