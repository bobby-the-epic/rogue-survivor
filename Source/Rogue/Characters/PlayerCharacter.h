// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Humanoid.h"
#include "Logging/LogMacros.h"
#include "Rogue/UI/EUpgradeType.h"
#include "Rogue/UI/PlayerHUD.h"
#include "PlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class AArrowProjectile;
class UArrowComponent;
class USphereComponent;
class UUpgradeScreen;
class UPauseMenu;
class UDeathMenu;
struct FInputActionValue;
class UGameOverMenu;
class AAxeDefense;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class APlayerCharacter : public AHumanoid
{
    GENERATED_BODY()

  private:
    float ZoomedInFOV;
    float ZoomedOutFOV;
    float ElapsedTime = 0;

    UPROPERTY(VisibleAnywhere)
    int32 CurrentExperience = 0;

    UPROPERTY(EditDefaultsOnly)
    int32 MaxExperience = 1000;

    UPROPERTY(VisibleAnywhere)
    int32 Level = 1;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bIsAiming = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bUseWeapon = false;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    bool bWeaponReady = true;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bGameOver = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bFullAutoFire = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    bool bMultishotEnabled = false;

    UPROPERTY(VisibleAnywhere)
    bool bBombUpgradeEnabled = false;

    UPROPERTY(VisibleAnywhere)
    bool bKnockbackEnabled = false;

    UPROPERTY(VisibleDefaultsOnly)
    USphereComponent* SphereCollider;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class AArrowProjectile> ProjectileClass;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class ABomb> BombClass;

    UPROPERTY()
    FTimerHandle BombTimer;

    UPROPERTY(EditAnywhere)
    FVector ArrowOffset;

    UPROPERTY()
    AArrowProjectile* PlaceholderArrow;

    UPROPERTY(VisibleDefaultsOnly)
    UArrowComponent* LeftArrowPos;

    UPROPERTY(VisibleDefaultsOnly)
    UArrowComponent* MiddleArrowPos;

    UPROPERTY(VisibleDefaultsOnly)
    UArrowComponent* RightArrowPos;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<AAxeDefense> AxeDefenseClass;

    UPROPERTY()
    AAxeDefense* AxeDefense;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UPlayerHUD> PlayerHUDClass;

    UPROPERTY()
    UPlayerHUD* PlayerHUD;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UUpgradeScreen> UpgradeScreenClass;

    UPROPERTY()
    UUpgradeScreen* UpgradeScreen;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UPauseMenu> PauseMenuClass;

    UPROPERTY()
    UPauseMenu* PauseMenu;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UDeathMenu> DeathMenuClass;

    UPROPERTY()
    UDeathMenu* DeathMenu;

    UPROPERTY()
    FTimerHandle GameTimer;

    UPROPERTY(EditDefaultsOnly)
    TSubclassOf<UGameOverMenu> GameOverMenuClass;

    UPROPERTY()
    UGameOverMenu* GameOverMenu;

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

    UPROPERTY(EditDefaultsOnly, Category = Input)
    UInputAction* PauseAction;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* CollectExperienceSound;

    UPROPERTY(EditDefaultsOnly)
    USoundBase* LevelUpSound;

  public:
    APlayerCharacter();

    virtual void TakeDamage(int32 Damage) override;
    void ApplyUpgrade(EUpgradeType UpgradeType);

    /** Returns CameraBoom subobject **/
    FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
    /** Returns FollowCamera subobject **/
    FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
    UUpgradeScreen* GetUpgradeScreen() const { return UpgradeScreen; }

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
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

  private:
    void ZoomIn();
    void ZoomOut();
    void SetCameraFOV();
    void Attack();
    void RotateCharacter();
    void StopFiring();
    void StartDeathState();

    UFUNCTION()
    void ResetRotation();

    UFUNCTION()
    void AddExperience();

    UFUNCTION()
    void StartLaunchingBombs();

    UFUNCTION()
    void LaunchBombs();

    UFUNCTION()
    void TogglePauseMenu();

    UFUNCTION()
    void UpdateTimer() { PlayerHUD->UpdateTimer(); }

    UFUNCTION()
    void GameOver();
};
