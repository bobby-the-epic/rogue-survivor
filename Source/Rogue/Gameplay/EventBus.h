#pragma once

#include "EventBus.generated.h"

UCLASS()
class ROGUE_API UEventBus : public UObject
{
    GENERATED_BODY()

    UDELEGATE()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMovedSignature, FVector, Location);

    UDELEGATE()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeathSignature);

  public:
    static UEventBus* Get();
    FOnPlayerMovedSignature OnPlayerMovedDelegate;
    FOnPlayerDeathSignature OnPlayerDeathDelegate;

  private:
    static UEventBus* Instance;
};
