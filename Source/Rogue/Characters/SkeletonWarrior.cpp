#include "SkeletonWarrior.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "PlayerCharacter.h"
#include "Rogue/Gameplay/EventBus.h"
#include "Rogue/UI/EnemyHealthBar.h"

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
    // Remove the function from the delegate when destroyed and not dead
    if (!IsDead)
    {
        UEventBus::Get()->OnPlayerMovedDelegate.RemoveDynamic(this, &ASkeletonWarrior::UpdateHealthBarRotation);
    }
    Super::BeginDestroy();
}
void ASkeletonWarrior::TakeDamage(int32 Damage)
{
    if (IsDead)
    {
        // TakeDamage shouldn't be called when the actor is dead because collision
        // is disabled on death, but I left this here just in case.
        return;
    }
    // Subtracts damage from current health and updates the health bar
    if (!HealthBarWidgetComponent->IsVisible())
    {
        HealthBarWidgetComponent->SetVisibility(true);
    }
    CurrentHealth -= Damage;
    if (CurrentHealth <= 0)
    {
        IsDead = true;
        UEventBus::Get()->OnPlayerMovedDelegate.RemoveDynamic(this, &ASkeletonWarrior::UpdateHealthBarRotation);
        HealthBarWidgetComponent->DestroyComponent();
        SetActorEnableCollision(false);
        // Sets a timer to destroy the actor so the body lingers for a few seconds.
        FTimerHandle TimerHandle;
        GetController()->Destroy();
        GetWorldTimerManager().SetTimer(TimerHandle, [this] { Destroy(); }, 5.0f, false);
        return;
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
