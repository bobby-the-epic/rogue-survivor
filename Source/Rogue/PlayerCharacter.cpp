// Copyright Epic Games, Inc. All Rights Reserved.

// This class is based on the default third person template

#include "PlayerCharacter.h"
#include "ArrowProjectile.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "CoreMinimal.h"
#include "DrawDebugHelpers.h"
#include "Engine/LocalPlayer.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputActionValue.h"

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
}
void APlayerCharacter::BeginPlay()
{
    // Call the base class
    Super::BeginPlay();

    GetMesh()->HideBoneByName(FName("Knife"), PBO_None);
    GetMesh()->HideBoneByName(FName("Knife_Offhand"), PBO_None);
    GetMesh()->HideBoneByName(FName("Throwable"), PBO_None);
    GetMesh()->HideBoneByName(FName("1H_Crossbow"), PBO_None);
}
void APlayerCharacter::Tick(float DeltaTime)
{
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

    if (Controller != nullptr && !bIsDead)
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
    }
}
void APlayerCharacter::ZoomIn()
{
    if (!bIsDead)
    {
        GetCharacterMovement()->bOrientRotationToMovement = false;
        GetCharacterMovement()->bUseControllerDesiredRotation = true;
        bIsAiming = true;
        SetCameraFOV();
    }
}
void APlayerCharacter::ZoomOut()
{
    if (!bIsDead)
    {
        ElapsedTime = 0;
        GetCharacterMovement()->bOrientRotationToMovement = true;
        GetCharacterMovement()->bUseControllerDesiredRotation = false;
        bIsAiming = false;
        SetCameraFOV();
    }
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
    if (!bIsDead && bWeaponReady)
    {
        bUseWeapon = true;
        GetWorldTimerManager().SetTimerForNextTick([this] { bUseWeapon = false; });
        bWeaponReady = false;

        FVector CameraLocation;
        FRotator CameraRotation;
        FVector LaunchDirection;
        GetActorEyesViewPoint(CameraLocation, CameraRotation);
        CameraLocation = CameraBoom->GetSocketLocation(CameraBoom->SocketName);
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;
        SpawnParams.Instigator = GetInstigator();
        FTransform CrossbowTransform = GetMesh()->GetBoneTransform("2H_Crossbow");

        // Line trace
        FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 10000);
        FCollisionQueryParams TraceParams(FName(TEXT("LineTrace")), true, this);
        FHitResult HitResult;
        GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, TraceParams);
        if (HitResult.bBlockingHit)
        {
            // DrawDebugLine(GetWorld(), CameraLocation, HitResult.ImpactPoint, FColor::Green, false, 2.0f);
            LaunchDirection = (HitResult.ImpactPoint - CrossbowTransform.GetLocation()).GetSafeNormal();
        }
        else
        {
            // DrawDebugLine(GetWorld(), CameraLocation, TraceEnd, FColor::Red, false, 2.0f);
            LaunchDirection = (TraceEnd - CrossbowTransform.GetLocation()).GetSafeNormal();
        }
        // Spawn the arrow and fire it in the launch direction
        AArrowProjectile* Projectile = GetWorld()->SpawnActor<AArrowProjectile>(
            ProjectileClass, CrossbowTransform.GetLocation(), LaunchDirection.Rotation(), SpawnParams);
        Projectile->FireInDirection(LaunchDirection);
        if (!bIsAiming)
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
    GetCharacterMovement()->bOrientRotationToMovement = false;
    GetCharacterMovement()->bUseControllerDesiredRotation = true;
    GetWorldTimerManager().SetTimer(
        TimerHandle,
        [this] {
            GetCharacterMovement()->bOrientRotationToMovement = true;
            GetCharacterMovement()->bUseControllerDesiredRotation = false;
        },
        0.35f, false);
}
bool APlayerCharacter::CanJumpInternal_Implementation() const
{
    if (bIsDead)
        return false;
    return Super::CanJumpInternal_Implementation();
}
