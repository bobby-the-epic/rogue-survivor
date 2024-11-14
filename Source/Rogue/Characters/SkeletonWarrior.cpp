#include "SkeletonWarrior.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Rogue/Gameplay/EventBus.h"
#include "Rogue/Gameplay/ExperienceOrb.h"
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
    UEventBus::Get()->OnPlayerDeathDelegate.AddDynamic(this, &ASkeletonWarrior::Celebrate);
    HealthBarWidget = Cast<UEnemyHealthBar>(HealthBarWidgetComponent->GetWidget());
    if (SpawnMontage)
    {
        PlayAnimMontage(SpawnMontage);
        FTimerHandle TimerHandle;
        /*
            The default mesh location is below the ground.
            This sets a timer to move the mesh up.
            Otherwise, the animation will flicker from the state machine to the montage.
            This workaround hides the flickering.
        */
        GetWorldTimerManager().SetTimer(
            TimerHandle, [this] { GetMesh()->SetRelativeLocation(FVector(0, 0, -65.0f)); }, 0.2f, false);
        GetMesh()->GetAnimInstance()->OnMontageEnded.AddDynamic(this, &ASkeletonWarrior::EndSpawning);
    }
}
void ASkeletonWarrior::EndPlay(EEndPlayReason::Type EndPlayReason)
{
    // Remove the function from the delegate when destroyed and not dead
    if (!IsDead)
    {
        UEventBus::Get()->OnPlayerMovedDelegate.RemoveDynamic(this, &ASkeletonWarrior::UpdateHealthBarRotation);
    }
    UEventBus::Get()->OnPlayerDeathDelegate.RemoveDynamic(this, &ASkeletonWarrior::Celebrate);

    Super::EndPlay(EndPlayReason);
}
void ASkeletonWarrior::EndSpawning(UAnimMontage* Montage, bool bInterrupted)
{
    // Run the behavior tree and set the player blackboard key
    // when the spawn animation is done playing.
    if (!IsPlayerDead)
    {
        AAIController* AIController = Cast<AAIController>(GetController());
        ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
        AIController->RunBehaviorTree(BehaviorTree);
        AIController->GetBlackboardComponent()->SetValueAsObject(TEXT("Player"), Player);
    }
    GetMesh()->GetAnimInstance()->OnMontageEnded.RemoveDynamic(this, &ASkeletonWarrior::EndSpawning);
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
        FVector SpawnLocation = GetActorLocation();
        SpawnLocation.Z = 0;
        GetWorld()->SpawnActor<AExperienceOrb>(ExperienceOrbBP, SpawnLocation, FRotator::ZeroRotator);
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
void ASkeletonWarrior::Attack() const
{
    // Checks if the player is overlapping the weapon collider.
    // If they are, then the player takes damage.
    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (WeaponCollider->IsOverlappingActor(Player))
    {
        Cast<ICombatInterface>(Player)->TakeDamage(WeaponDamage);
    }
}
void ASkeletonWarrior::Celebrate()
{
    IsPlayerDead = true;
    GetController()->Destroy();
}
