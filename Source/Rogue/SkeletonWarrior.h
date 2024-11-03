#pragma once

#include "CombatInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SkeletonWarrior.generated.h"

UCLASS()
class ROGUE_API ASkeletonWarrior : public ACharacter, public ICombatInterface
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere)
    int32 Health = 100;

    UPROPERTY(VisibleDefaultsOnly)
    UStaticMeshComponent* WeaponMesh;

    UPROPERTY(VisibleDefaultsOnly)
    UStaticMeshComponent* ShieldMesh;

    UPROPERTY(VisibleDefaultsOnly)
    UCapsuleComponent* WeaponCollider;

  public:
    ASkeletonWarrior();
    virtual void TakeDamage(int32 Damage) override;
};
