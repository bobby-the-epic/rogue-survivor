#include "SkeletonWarrior.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "EnemyHealthBar.h"
#include "EventBus.h"
#include "PlayerCharacter.h"

ASkeletonWarrior::ASkeletonWarrior()
{
    PrimaryActorTick.bCanEverTick = false;

    // Initialize the components and attach them to the appropriate sockets in the mesh.
    WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMesh");
    WeaponMesh->SetupAttachment(GetMesh(), "handslot_r");
    ShieldMesh = CreateDefaultSubobject<UStaticMeshComponent>("ShieldMesh");
    ShieldMesh->SetupAttachment(GetMesh(), "handslot_l");
    WeaponCollider = CreateDefaultSubobject<UCapsuleComponent>("WeaponCollider");
    WeaponCollider->SetupAttachment(GetMesh(), "handslot_r");
    HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthBarWidget");
    HealthBarWidgetComponent->SetupAttachment(RootComponent);
}
void ASkeletonWarrior::BeginPlay()
{
    Super::BeginPlay();
    UEventBus::Get()->OnPlayerMovedDelegate.AddDynamic(this, &ASkeletonWarrior::UpdateHealthBarRotation);
    HealthBarWidget = Cast<UEnemyHealthBar>(HealthBarWidgetComponent->GetWidget());
}
void ASkeletonWarrior::BeginDestroy()
{
    // Remove the function from the delegate when destroyed
    UEventBus::Get()->OnPlayerMovedDelegate.RemoveDynamic(this, &ASkeletonWarrior::UpdateHealthBarRotation);
    Super::BeginDestroy();
}
void ASkeletonWarrior::TakeDamage(int32 Damage)
{
    // Subtracts damage from current health and updates the health bar
    if (!HealthBarWidgetComponent->IsVisible())
    {
        HealthBarWidgetComponent->SetVisibility(true);
    }
    CurrentHealth -= Damage;
    if (CurrentHealth <= 0)
    {
        Destroy();
    }
    HealthBarWidget->HealthBar->SetPercent(CurrentHealth / MaxHealth);
    // For debugging
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red,
                                     FString::Printf(TEXT("%f"), HealthBarWidget->HealthBar->GetPercent()));
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("%f"), CurrentHealth));
}
void ASkeletonWarrior::UpdateHealthBarRotation(FVector CameraLocation)
{
    // Rotates the health bar to face the player, but it is only called
    // when the player moves.
    HealthBarWidgetComponent->SetWorldRotation(
        (CameraLocation - HealthBarWidgetComponent->GetComponentLocation()).Rotation());
}
