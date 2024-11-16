// Copyright Epic Games, Inc. All Rights Reserved.

// This class is based on the default third person template

#include "PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ProgressBar.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Rogue/Gameplay/ArrowProjectile.h"
#include "Rogue/Gameplay/EventBus.h"
#include "Rogue/Gameplay/MainGameMode.h"
#include "Rogue/UI/PlayerHUD.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APlayerCharacter
APlayerCharacter::APlayerCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 55.0f);

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;            // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

    // Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
    // instead of recompiling to adjust them
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;       // The camera follows at this distance behind the character
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(
        CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom
                                                      // adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false;    // Camera does not rotate relative to arm

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
    // are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

    ZoomedOutFOV = FollowCamera->FieldOfView;
    ZoomedInFOV = ZoomedOutFOV - 30;

    LeftArrowPos = CreateDefaultSubobject<UArrowComponent>("LeftArrowPos");
    LeftArrowPos->SetupAttachment(RootComponent);
    MiddleArrowPos = CreateDefaultSubobject<UArrowComponent>("MiddleArrowPos");
    MiddleArrowPos->SetupAttachment(RootComponent);
    RightArrowPos = CreateDefaultSubobject<UArrowComponent>("RightArrowPos");
    RightArrowPos->SetupAttachment(RootComponent);

    SphereCollider = CreateDefaultSubobject<USphereComponent>("SphereCollider");
    SphereCollider->SetupAttachment(RootComponent);
}
void APlayerCharacter::BeginPlay()
{
    // Call the base class
    Super::BeginPlay();

    // Hides all the unused weapons from the mesh.
    GetMesh()->HideBoneByName(FName("Knife"), PBO_None);
    GetMesh()->HideBoneByName(FName("Knife_Offhand"), PBO_None);
    GetMesh()->HideBoneByName(FName("Throwable"), PBO_None);
    GetMesh()->HideBoneByName(FName("1H_Crossbow"), PBO_None);

    // Spawns a placeholder arrow to simulate reloading the crossbow.
    // The arrow's visibility is toggled off and on when the player reloads.
    // There is no arrow in the reload animation, so I did it with code instead.

    FVector CrossbowLocation = GetMesh()->GetBoneLocation("2H_Crossbow");
    FRotator CrossbowRotation = GetMesh()->GetBoneTransform("2H_Crossbow").Rotator();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = this;
    PlaceholderArrow =
        GetWorld()->SpawnActor<AArrowProjectile>(ProjectileClass, CrossbowLocation, CrossbowRotation, SpawnParams);
    PlaceholderArrow->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
                                        "2H_Crossbow");
    PlaceholderArrow->SetActorRelativeLocation(ArrowOffset);
    PlaceholderArrow->ProjectileMovementComponent->DestroyComponent();
    PlaceholderArrow->SetActorEnableCollision(false);
    PlaceholderArrow->SetLifeSpan(0);

    if (PlayerHUDClass)
    {
        PlayerHUD = CreateWidget<UPlayerHUD>(Cast<APlayerController>(GetController()), PlayerHUDClass);
        PlayerHUD->AddToPlayerScreen();
    }

    EventBus = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetEventBus();
    EventBus->OnCollectiblePickupDelegate.AddDynamic(this, &APlayerCharacter::AddExperience);
}
void APlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (PlayerHUD)
    {
        PlayerHUD->RemoveFromParent();
        PlayerHUD = nullptr;
    }
    EventBus->OnCollectiblePickupDelegate.RemoveDynamic(this, &APlayerCharacter::AddExperience);

    Super::EndPlay(EndPlayReason);
}
//////////////////////////////////////////////////////////////////////////
// Input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
                ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    // Set up action bindings
    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {

        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacter::ZoomIn);
        EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &APlayerCharacter::ZoomOut);
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopFiring);
        EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Canceled, this, &APlayerCharacter::StopFiring);
    }
    else
    {
        UE_LOG(LogTemplateCharacter, Error,
               TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input "
                    "system. If you intend to use the legacy system, then you will need to update this C++ file."),
               *GetNameSafe(this));
    }
}
void APlayerCharacter::Move(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // get right vector
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);

        EventBus->OnPlayerMovedDelegate.Broadcast(FollowCamera->GetComponentLocation());
    }
}
void APlayerCharacter::Look(const FInputActionValue& Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);

        EventBus->OnPlayerMovedDelegate.Broadcast(FollowCamera->GetComponentLocation());
    }
}
void APlayerCharacter::ZoomIn()
{
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    bIsAiming = true;
    SetCameraFOV();
}
void APlayerCharacter::ZoomOut()
{
    ElapsedTime = 0;
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->bUseControllerDesiredRotation = false;
    bIsAiming = false;
    SetCameraFOV();
}
void APlayerCharacter::SetCameraFOV()
{
    if (bIsAiming && FollowCamera->FieldOfView != ZoomedInFOV)
    {
        FollowCamera->FieldOfView = FMath::FInterpTo(FollowCamera->FieldOfView, ZoomedInFOV, FApp().GetDeltaTime(), 5);
        GetWorldTimerManager().SetTimerForNextTick(this, &APlayerCharacter::SetCameraFOV);
    }
    else if (!bIsAiming && FollowCamera->FieldOfView != ZoomedOutFOV)
    {
        FollowCamera->FieldOfView = FMath::FInterpTo(FollowCamera->FieldOfView, ZoomedOutFOV, FApp().GetDeltaTime(), 5);
        GetWorldTimerManager().SetTimerForNextTick(this, &APlayerCharacter::SetCameraFOV);
    }
}
void APlayerCharacter::Attack()
{
    if (bWeaponReady)
    {
        bUseWeapon = true;
        bWeaponReady = false;
        GetCharacterMovement()->bOrientRotationToMovement = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        if (!bFullAutoFire && !bIsAiming)
        {
            RotateCharacter();
        }
    }
}
void APlayerCharacter::FireArrow()
{
    FVector CameraLocation = CameraBoom->GetSocketLocation(CameraBoom->SocketName);
    FRotator CameraRotation = CameraBoom->GetSocketRotation(CameraBoom->SocketName);
    FVector LaunchDirection;
    FVector MiddleArrowLocation = MiddleArrowPos->GetComponentLocation();
    FRotator MiddleArrowRotation = MiddleArrowPos->GetComponentRotation();

    // Line trace
    FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 1000);
    FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, this);
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, TraceParams);
        LaunchDirection = (TraceEnd - MiddleArrowLocation).GetSafeNormal();
    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = this;
    if (!bFullAutoFire)
    {
        PlaceholderArrow->SetActorHiddenInGame(true);
    }
    // Spawn the arrow and fire it in the launch direction
    AArrowProjectile* Arrow = GetWorld()->SpawnActor<AArrowProjectile>(ProjectileClass, MiddleArrowLocation,
                                                                       MiddleArrowRotation, SpawnParams);
    Arrow->SetWeaponDamage(WeaponDamage);
    Arrow->FireInDirection(LaunchDirection);
    if (bMultishotEnabled)
    {
        // Spawns the arrows with the arrow components' locations and rotations
        FVector LeftArrowLocation = LeftArrowPos->GetComponentLocation();
        FVector RightArrowLocation = RightArrowPos->GetComponentLocation();
        FRotator LeftArrowRotation = LeftArrowPos->GetComponentRotation();
        FRotator RightArrowRotation = RightArrowPos->GetComponentRotation();
        // Just rotates the launch direction a little so they fire in different directions
        FVector LeftDirection = LaunchDirection.RotateAngleAxis(-25, FVector::UpVector);
        FVector RightDirection = LaunchDirection.RotateAngleAxis(25, FVector::UpVector);
        AArrowProjectile* LeftArrow = GetWorld()->SpawnActor<AArrowProjectile>(ProjectileClass, LeftArrowLocation,
                                                                               LeftArrowRotation, SpawnParams);
        AArrowProjectile* RightArrow = GetWorld()->SpawnActor<AArrowProjectile>(ProjectileClass, RightArrowLocation,
                                                                                RightArrowRotation, SpawnParams);
        LeftArrow->SetWeaponDamage(WeaponDamage);
        RightArrow->SetWeaponDamage(WeaponDamage);
        LeftArrow->FireInDirection(LeftDirection);
        RightArrow->FireInDirection(RightDirection);
    }
}
void APlayerCharacter::ReloadArrow()
{
    PlaceholderArrow->SetActorHiddenInGame(false);
}
void APlayerCharacter::StopFiring()
{
    bUseWeapon = false;
    bWeaponReady = true;
    if (bFullAutoFire && !bIsAiming)
    {
        RotateCharacter();
    }
}
/*
        Rotates the character in the direction the camera is facing when
        the player shoots without aiming (AKA hip fire).
*/
void APlayerCharacter::RotateCharacter()
{
    FTimerHandle TimerHandle;
    GetWorldTimerManager().SetTimer(
        TimerHandle,
        [this] {
            GetCharacterMovement()->bOrientRotationToMovement = true;
            GetCharacterMovement()->bUseControllerDesiredRotation = false;
        },
        0.35f, false);
}
void APlayerCharacter::TakeDamage(int32 Damage)
{
    if (bIsDead)
    {
        return;
    }
    CurrentHealth -= Damage;
    PlayerHUD->HealthBar->SetPercent(CurrentHealth / MaxHealth);
    if (CurrentHealth <= 0)
    {
        StartDeathState();
    }
}
void APlayerCharacter::StartDeathState()
{
    bIsDead = true;
    EventBus->OnPlayerDeathDelegate.Broadcast();
    DisableInput(Cast<APlayerController>(GetController()));
    //  Creates a "death cam" (not actually a camera, just an actor with a placeholder mesh)
    //  and moves the camera view to it smoothly with the SetViewTargetWithBlend function.
    FVector DeathCamLocation = FVector(175, 60, 200) + FollowCamera->GetComponentLocation();
    DeathCamLocation = GetMesh()->GetComponentLocation() + FVector(0, 0, 400);
    FRotator DeathCamRotation = FRotator(-90, 0, 0);
    AActor* DeathCam = GetWorld()->SpawnActor<AActor>(AActor::StaticClass());
    UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(DeathCam);
    DeathCam->SetRootComponent(StaticMeshComponent);
    StaticMeshComponent->RegisterComponent();
    DeathCam->SetActorLocation(DeathCamLocation);
    DeathCam->SetActorRotation(DeathCamRotation);
    Cast<APlayerController>(GetController())
        ->SetViewTargetWithBlend(DeathCam, 6.0f, EViewTargetBlendFunction::VTBlend_EaseOut, 3.0f);
}
void APlayerCharacter::AddExperience()
{
    CurrentExperience += 100;
    if (CurrentExperience >= MaxExperience)
    {
        Level++;
        CurrentExperience = 0;
        MaxExperience += 100;
    }
}
