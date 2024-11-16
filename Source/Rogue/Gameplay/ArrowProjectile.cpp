#include "ArrowProjectile.h"
#include "CombatInterface.h"

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
        ICombatInterface* HitActor = Cast<ICombatInterface>(OtherActor);
        if (HitActor != nullptr)
        {
            HitActor->TakeDamage(WeaponDamage);
        }
    }
    else
    {
        SetActorEnableCollision(false);
    }
}
