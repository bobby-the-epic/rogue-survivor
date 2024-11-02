#include "SkeletonWarrior.h"
#include "Components/CapsuleComponent.h"

ASkeletonWarrior::ASkeletonWarrior()
{
    PrimaryActorTick.bCanEverTick = false;

    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
    WeaponMesh->SetupAttachment(GetMesh(), "handslot_r");
    ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShieldMesh");
    ShieldMesh->SetupAttachment(GetMesh(), "handslot_l");
    WeaponCollider = CreateDefaultSubobject<UCapsuleComponent>("WeaponCollider");
    WeaponCollider->SetupAttachment(GetMesh(), "handslot_r");
}
