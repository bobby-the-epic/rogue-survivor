#pragma once

#include "CoreMinimal.h"
#include "EUpgradeType.h"
#include "UObject/NoExportTypes.h"
#include "Upgrade.generated.h"

class UTexture2D;

UCLASS(Blueprintable, BlueprintType)
class ROGUE_API UUpgrade : public UObject
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    UTexture2D* UpgradeTexture;

    UPROPERTY(EditDefaultsOnly)
    EUpgradeType UpgradeType;

  public:
    FORCEINLINE UTexture2D* GetTexture() const { return UpgradeTexture; }
    FORCEINLINE EUpgradeType GetType() const { return UpgradeType; }
};
