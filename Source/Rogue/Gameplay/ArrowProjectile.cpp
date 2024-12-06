#include "ArrowProjectile.h"
#include "Rogue/Characters/SkeletonWarrior.h"

AArrowProjectile::AArrowProjectile()
{
    PrimaryActorTick.bCanEverTick = false;

    ArrowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowMesh"));
    RootComponent = ArrowMesh;
    ProjectileMovementComponent =
        CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}
void AArrowProjectile::BeginPlay()
{
    Super::BeginPlay();
    ArrowMesh->OnComponentHit.AddDynamic(this, &AArrowProjectile::OnHit);
}
void AArrowProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // Unsubscribe from the delegate
    ArrowMesh->OnComponentHit.RemoveDynamic(this, &AArrowProjectile::OnHit);
    Super::EndPlay(EndPlayReason);
}
void AArrowProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}
void AArrowProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                             FVector NormalImpulse, const FHitResult& Hit)
{
    if (OtherActor->IsA<APawn>())
    {
        Destroy();
        ASkeletonWarrior* HitActor = Cast<ASkeletonWarrior>(OtherActor);
        if (HitActor != nullptr)
        {
            if (bHasKnockback)
            {
                HitActor->Knockback();
            }
            HitActor->TakeDamage(WeaponDamage);
        }
    }
    else
    {
        SetActorEnableCollision(false);
    }
}
