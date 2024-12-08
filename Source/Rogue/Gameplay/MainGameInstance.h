#pragma once

#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/AmbientSound.h"
#include "MainGameInstance.generated.h"

class USoundAttenuation;

UCLASS()
class ROGUE_API UMainGameInstance : public UGameInstance
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    int32 TimerMinutes = 30;

    UPROPERTY(EditDefaultsOnly)
    float MusicVolume = 0.5f;

    UPROPERTY(EditDefaultsOnly)
    float SFXVolume = 0.5f;

    UPROPERTY(EditDefaultsOnly)
    USoundAttenuation* SoundAttenuation;

  public:
    int32 GetTimerMinutes() const { return TimerMinutes; }
    USoundAttenuation* GetSoundAttenuation() const { return SoundAttenuation; }
    float GetMusicVolume() const { return MusicVolume; }
    void SetMusicVolume(const float NewVolume) { MusicVolume = NewVolume; }
    void SetSFXVolume(const float NewVolume) { SFXVolume = NewVolume; }

    UFUNCTION(BlueprintCallable)
    float GetSFXVolume() const { return SFXVolume; }

    UFUNCTION(BlueprintCallable)
    void SetTimerMinutes(const int32 NewMinutes) { TimerMinutes = NewMinutes; }
};
