#pragma once

#include "MainGameModeBase.h"
#include "MainGameMode.generated.h"

class UEventBus;
class APlayerCharacter;

UCLASS(minimalapi)
class AMainGameMode : public AMainGameModeBase
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere)
    int32 SkeletonWeaponDamage = 5;

    UPROPERTY(VisibleAnywhere)
    int32 SkeletonHealth = 30;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class ASkeletonWarrior> SkeletonClass;

    UPROPERTY()
    FTimerHandle SpawnTimerHandle;

    UPROPERTY()
    FTimerHandle StatIncreaseTimerHandle;

    UPROPERTY()
    FTimerHandle HalftimeTimer;

    UPROPERTY()
    UEventBus* EventBus;

    UPROPERTY()
    APlayerCharacter* Player;

  public:
    FORCEINLINE UEventBus* GetEventBus() const { return EventBus; }

  protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  private:
    UFUNCTION()
    void SpawnSkeleton() const;

    UFUNCTION()
    void StopSpawningSkeletons();

    UFUNCTION()
    void GameOver();

    UFUNCTION()
    void IncreaseSkeletonStats();

    UFUNCTION()
    void IncreaseSpawnRate();
};
