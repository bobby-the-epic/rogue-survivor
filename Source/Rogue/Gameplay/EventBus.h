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

    UDELEGATE()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCollectiblePickupSignature);

    UDELEGATE()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerLevelUpSignature);

    UDELEGATE()
    DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBombUpgradeChosenSignature);

  public:
    UPROPERTY()
    FOnPlayerMovedSignature OnPlayerMovedDelegate;

    UPROPERTY()
    FOnPlayerDeathSignature OnPlayerDeathDelegate;

    UPROPERTY()
    FOnCollectiblePickupSignature OnCollectiblePickupDelegate;

    UPROPERTY()
    FOnPlayerLevelUpSignature OnPlayerLevelUpDelegate;

    UPROPERTY()
    FOnBombUpgradeChosenSignature OnBombUpgradeChosenDelegate;
};
