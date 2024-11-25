#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
    GENERATED_BODY()
};

class ROGUE_API ICombatInterface
{
    GENERATED_BODY()

  public:
    virtual void TakeDamage(int32 Damage) {}
    virtual void Knockback() {}
};
