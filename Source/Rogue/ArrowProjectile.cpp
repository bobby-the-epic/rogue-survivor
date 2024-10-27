#include "ArrowProjectile.h"

// Sets default values
AArrowProjectile::AArrowProjectile()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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
void AArrowProjectile::FireInDirection(const FVector& ShootDirection)
{
    ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}
void AArrowProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
                             FVector NormalImpulse, const FHitResult& Hit)
{
    GEngine->AddOnScreenDebugMessage(-1, 1, FColor::White, TEXT("Hit"));
}
