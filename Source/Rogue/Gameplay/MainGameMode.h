#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainGameMode.generated.h"

UCLASS(minimalapi)
class AMainGameMode : public AGameModeBase
{
    GENERATED_BODY()

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class ASkeletonWarrior> SkeletonClass;

  protected:
    virtual void BeginPlay() override;

  private:
    UFUNCTION()
    void SpawnSkeleton() const;
};
