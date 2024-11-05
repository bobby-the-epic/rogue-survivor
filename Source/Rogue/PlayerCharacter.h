// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CombatInterface.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AArrowProjectile;
class UArrowComponent;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMovedSignature, FVector, Location);

UCLASS(config = Game)
class APlayerCharacter : public ACharacter, public ICombatInterface
{
    GENERATED_BODY()

  public:
    FOnPlayerMovedSignature OnPlayerMovedDelegate;

  private:
    float ZoomedInFOV;
    float ZoomedOutFOV;
    float ElapsedTime = 0;

    UPROPERTY(VisibleAnywhere)
    float CurrentHealth = 100;

    UPROPERTY(VisibleAnywhere)
    float MaxHealth = 100;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bIsAiming = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bUseWeapon = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bIsDead = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    bool bWeaponReady = true;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bFullAutoFire = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bMultishotEnabled = false;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class AArrowProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
    FVector ArrowOffset;

    UPROPERTY()
    AArrowProjectile* PlaceholderArrow;

    UPROPERTY(VisibleDefaultsOnly)
    UArrowComponent* LeftArrowPos;

    UPROPERTY(VisibleDefaultsOnly)
    UArrowComponent* MiddleArrowPos;

    UPROPERTY(VisibleDefaultsOnly)
    UArrowComponent* RightArrowPos;

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    /** MappingContext */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    /** Jump Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

    /** Move Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    /** Look Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* AimAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* ShootAction;

  public:
    APlayerCharacter();
    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

    virtual void TakeDamage(int32 Damage) override;

    UFUNCTION(BlueprintCallable)
    void FireArrow();

    UFUNCTION(BlueprintCallable)
    void ReloadArrow();

  protected:
    /** Called for movement input */
    void Move(const FInputActionValue& Value);
    /** Called for looking input */
    void Look(const FInputActionValue& Value);
    // APawn interface
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // To add mapping context
    virtual void BeginPlay() override;
    virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;
    virtual bool CanJumpInternal_Implementation() const override;

  private:
    void ZoomIn();
    void ZoomOut();
    void SetCameraFOV();
    void Attack();
    void RotateCharacter();
    void StopFiring();
};
