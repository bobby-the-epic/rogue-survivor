#pragma once

#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameInstance.h"
#include "Sound/AmbientSound.h"
#include "MainGameModeBase.generated.h"

UCLASS(Abstract, minimalapi)
class AMainGameModeBase : public AGameModeBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AAmbientSound> AmbientSoundClass;

    UPROPERTY()
    AAmbientSound* MusicActor;

  public:
    void SetMusicVolume(const float NewVolume) { MusicActor->GetAudioComponent()->SetVolumeMultiplier(NewVolume); }

  protected:
    virtual void BeginPlay() override
    {
        Super::BeginPlay();
        MusicActor = GetWorld()->SpawnActor<AAmbientSound>(AmbientSoundClass, FVector::Zero(), FRotator::ZeroRotator);
        MusicActor->GetAudioComponent()->SetVolumeMultiplier(GetGameInstance<UMainGameInstance>()->GetMusicVolume());
    }
};
