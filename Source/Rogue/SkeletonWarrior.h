#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkeletonWarrior.generated.h"

UCLASS()
class ROGUE_API ASkeletonWarrior : public ACharacter
{
    GENERATED_BODY()

    UPROPERTY(VisibleDefaultsOnly)
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleDefaultsOnly)
    UStaticMeshComponent* ShieldMesh;

  public:
    ASkeletonWarrior();
};
