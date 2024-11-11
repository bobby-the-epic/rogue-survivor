#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

class UEventBus;

UCLASS(minimalapi)
class AMainGameMode : public AGameModeBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class ASkeletonWarrior> SkeletonClass;

    UPROPERTY()
    FTimerHandle SpawnTimerHandle;

    UPROPERTY()
    UEventBus* EventBus;

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  private:
    UFUNCTION()
    void SpawnSkeleton() const;

    UFUNCTION()
    void StopSpawningSkeletons();
};
