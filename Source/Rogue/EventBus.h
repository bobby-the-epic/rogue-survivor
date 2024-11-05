#pragma once

#include "EventBus.generated.h"

UCLASS()
class ROGUE_API UEventBus : public UObject
{
    GENERATED_BODY()

    UDELEGATE()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMovedSignature, FVector, Location);

  public:
    static UEventBus* Get();
    FOnPlayerMovedSignature OnPlayerMovedDelegate;

  private:
    static UEventBus* Instance;
};
