#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MainGameInstance.generated.h"

UCLASS()
class ROGUE_API UMainGameInstance : public UGameInstance
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    int32 TimerMinutes = 30;

  public:
    int32 GetTimerMinutes() const { return TimerMinutes; }

    UFUNCTION(BlueprintCallable)
    void SetTimerMinutes(int32 NewMinutes) { TimerMinutes = NewMinutes; }
};
