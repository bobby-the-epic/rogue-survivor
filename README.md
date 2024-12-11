<div align="center">
  <img src="https://github.com/user-attachments/assets/1cc4ee7c-9256-46fb-b328-427e53e94fda" width="300">
</div>

# Rogue Survivor

Rogue Survivor is a third-person shooter demo with gameplay similar to Vampire Survivors. 

There are three difficulties - each corresponding to how long the game timer runs for. The choices are: 10, 15, or 30 minutes.

The player spawns in with a semi-automatic crossbow. The goal of the game is to survive until the timer runs out.

The player can collect experience from defeated enemies to unlock powerful upgrades.

## Upgrades

<details>

<summary>Full Auto Fire</summary>

<div align="left">
  <img src="Assets/UI/Upgrades/full auto fire.png" width="209">
</div>

Turns the crossbow into full auto fire mode.
  
</details>

<details>

<summary>Multishot</summary>

<div align="left">
  <img src="Assets/UI/Upgrades/multishot upgrade.png" width="209">
</div>

Fires two additional arrows when the player shoots the crossbow.
  
</details>

<details>
  <summary>Knockback Arrows</summary>

  <div align="left">
  <img src="Assets/UI/Upgrades/knockback upgrade.png" width="209">
</div>

Knocks the enemy back when hit with an arrow.

</details>

<details>
  <summary>Axe Defense</summary>
  
  <div align="left">
  <img src="Assets/UI/Upgrades/axe upgrade.png" width="209">
</div>

Surrounds the player with a barrier of axes that damage any enemies that come close.
  
</details>

<details>
  <summary>Bomb Throw</summary>

  <div align="left">
  <img src="Assets/UI/Upgrades/bomb upgrade.png" width="209">
</div>

Throws four bombs every three seconds in a radius around the player.

</details>

<details>
  <summary>Movement Speed</summary>

  <div align="left">
  <img src="Assets/UI/Upgrades/movement speed upgrade.png" width="209">
</div>

Increases the player's movement speed by 25.

</details>

<details>
  <summary>Increased Health</summary>

<div align="left">
  <img src="Assets/UI/Upgrades/health upgrade.png" width="209">
</div>

Increases the player's max health by 10 and fully heals the player.
  
</details>

<details>
  <summary>Increased Damage</summary>

  <div align="left">
  <img src="Assets/UI/Upgrades/damage upgrade.png" width="209">
</div>

Increases the player's damage of all weapons by 5.

</details>

## Technical Details

### The Event Bus

No custom classes use the `Tick` function. All gameplay logic is event driven through
delegates in the `UEventBus` class in [EventBus.h](Source/Rogue/Gameplay/EventBus.h).


A `UEventBus` object is instantiated in the `AMainGameMode` class in [MainGameMode.cpp](Source/Rogue/Gameplay/MainGameMode.cpp):

```cpp
EventBus = NewObject<UEventBus>();
```

The `AMainGameMode` member variable `EventBus` is then used by other classes to bind methods to the delegate variables that reside in the `UEventBus` class through the use of `AMainGameMode`'s `GetEventBus` public function.


If a class needs to use a delegate variable, they first get the game mode, which is assigned by default to `AMainGameMode` (except on the main menu), and then they call the `AddDynamic` macro to bind a function in their `BeginPlay` function. A `UEventBus` variable is declared as a member variable in the class for convenience.

```cpp
// Humanoid.cpp

void AHumanoid::BeginPlay()
{
  EventBus = Cast<AMainGameMode>(UGameplayStatics::GetGameMode(GetWorld()))->GetEventBus();
  EventBus->OnSFXVolumeChangedDelegate.AddDynamic(this, &AHumanoid::SetAudioVolume);
}
```

In this example, the `EventBus` member variable of the [`AHumanoid` class](Source/Rogue/Characters/Humanoid.h) is initialized by getting the game mode with the `UGameplayStatics` library, casting it to an `AMainGameMode` object, and calling `GetEventBus` on that object, returning the main `UEventBus` object.

The event bus has a delegate variable named `OnSFXVolumeChangedDelegate` that is used to bind the `AHumanoid` function `SetAudioVolume` to it with the `AddDynamic` macro.

Now when a function calls `OnSFXVolumeChangedDelegate.Broadcast()`, all humanoid objects call their own `SetAudioVolume` function. This function is used to change the sound effect volume on all humanoids (both enemies and the player character itself) when the player changes the volume slider in the options menu.

All delegates and their variables are located in the [EventBus.h](Source/Rogue/Gameplay/EventBus.h) source file.


### The Player Character

The aptly named `APlayerCharacter` class, located in [PlayerCharacter.h](Source/Rogue/Characters/PlayerCharacter.h), is the primary source of interaction between the player and the game.

The class was originally from the character class of Epic Games' Third Person Template. It only included a basic setup of components in the constructor, along with some input handling.

For a while, it inherited from the `ACharacter` class, which is an Unreal class for playable or non-playable characters. There is only one enemy type in the game, but I realized later on that they shared much of the same functionality such as health, damage, sound effects, etc. I created a superclass for both the enemy type and the player to inherit from called `AHumanoid` (which inherits from `ACharacter`).
 
The player's abilites include:
- Moving the camera with the mouse
- Moving the character with the WASD keys
- Jumping
- Aiming
- Shooting arrows
- Launching bombs

Along with basic functionality such as playing sound effects and taking damage, these abilites make up the player character class.

Movement functions use the Enhanced Input subsystem and remain mostly unchanged from the Third Person Template.

#### Aiming

Aiming is straightforward and is done by linearly interpolating the camera's field of view.

```cpp
// PlayerCharacter.cpp

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
```

The boolean member variable `bIsAiming` is set when the player presses or lets go of the right mouse button.
The function checks whether the player wants to zoom in or out first and then checks if the camera's field
of view matches the member variable `ZoomedInFOV` or `ZoomedOutFOV`. `ZoomedInFOV` is set to 60 degrees and `ZoomedOutFOV`
is set to 90 degrees by default.

The camera's field of view is then set to an interpolated value. The interpolation starts with the camera's current
field of view and the target value is either `ZoomedInFOV` or `ZoomedOutFOV`. Delta time is passed in as a required argument
for the `FMath::FInterpTo` function. The integer 5 is passed in as the interpolation speed.

The function then sets a timer for the next tick to call itself and interpolate the value again, basically acting as a recursive function.
What this means is that if the player wants to aim, the function sets the camera's field of view every tick until it reaches 60 degrees.
When the player lets go of the right mouse button, the field of view is interpolated back every tick until 90 degrees is reached.

#### Shooting

The player character is armed with a semi-automatic crossbow that fires arrows when the player presses the left mouse button. The `FireArrow`
function casts a ray in the middle of the screen to a point in the distance. If the player has the multishot upgrade, two more arrows will fire.

```cpp
// PlayerCharacter.cpp
// Shortened for brevity

void APlayerCharacter::FireArrow()
{
    FVector CameraLocation;
    FRotator CameraRotation;
    Controller->GetPlayerViewPoint(CameraLocation, CameraRotation);
    FVector LaunchDirection;
    const FVector MiddleArrowLocation = MiddleArrowPos->GetComponentLocation();
    const FRotator MiddleArrowRotation = MiddleArrowPos->GetComponentRotation();

    // Line trace
    const FVector TraceEnd = CameraLocation + (CameraRotation.Vector() * 10000);
    const FCollisionQueryParams TraceParams(FName("LineTrace"), true, this);
    FHitResult HitResult;
    GetWorld()->LineTraceSingleByChannel(HitResult, CameraLocation, TraceEnd, ECC_Visibility, TraceParams);
    const FVector HitDirection = (HitResult.ImpactPoint - GetActorLocation()).GetSafeNormal();

    // If the line trace hits an enemy, and the enemy is not behind the player,
    // then the arrow is launched towards the hit enemy.
    if (HitResult.bBlockingHit && FVector::DotProduct(GetActorForwardVector(), HitDirection) > 0)
    {
        LaunchDirection = (HitResult.ImpactPoint - MiddleArrowLocation).GetSafeNormal();
    }
    else
    {
        LaunchDirection = (TraceEnd - MiddleArrowLocation).GetSafeNormal();
    }
    FActorSpawnParameters SpawnParams;
    SpawnParams.Instigator = this;
    
    // Spawn the arrow and fire it in the launch direction
    AArrowProjectile* Arrow = GetWorld()->SpawnActor<AArrowProjectile>(ProjectileClass, MiddleArrowLocation,
                                                                       MiddleArrowRotation, SpawnParams);
    Arrow->SetWeaponDamage(WeaponDamage);
    Arrow->FireInDirection(LaunchDirection);
    if (bKnockbackEnabled)
    {
        Arrow->SetKnockback(true);
    }
}
```

First, several local variables are initialized for casting the ray.
The `GetPlayerViewPoint` function is used to get the location and rotation of the player's camera
in world space. Then, a single line trace by channel is performed with the `LineTraceSingleByChannel`
function. The `HitDirection` local variable is the direction from the player to the point in world space
that the raycast hit. 

If the raycast hit something (either an enemy or a static mesh), the player's forward vector is used to determine
where the hit actor is.

```cpp
if (HitResult.bBlockingHit && FVector::DotProduct(GetActorForwardVector(), HitDirection) > 0)
```

If the dot product of the player's forward vector and `HitDirection` is less than zero, then that means the hit actor
is behind the player mesh. This check is necessary because an enemy can get between the player mesh and the camera. If
the player tries to shoot when that happens, the arrow will appear to fire backwards, instead of shooting forwards out of
the crossbow.

The remaining lines of code spawn the arrow and set the arrow's stats. The multishot implementation is omitted here, but it
is mostly the same except the two extra arrows are rotated in slightly different directions.

#### Launching Bombs

When the player chooses the Bomb Throw upgrade, a timer is set in the `StartLaunchingBombs` function. This timer runs every three
seconds and calls the `LaunchBombs` function.

```cpp
// PlayerCharacter.cpp

void APlayerCharacter::LaunchBombs()
{
    // Spawn the bomb above the player character's head
    FVector SpawnLocation = GetActorLocation();
    SpawnLocation.Z += 30;
    FVector ForwardDirection = GetActorForwardVector();
    FVector RightDirection = GetActorRightVector();
    // Set the directions for the bomb to launch in
    FVector Directions[4];
    Directions[0] = ForwardDirection;
    Directions[1] = -ForwardDirection;
    Directions[2] = RightDirection;
    Directions[3] = -RightDirection;

    for (int Counter = 0; Counter < 4; Counter++)
    {
        ABomb* Bomb = GetWorld()->SpawnActor<ABomb>(BombClass, SpawnLocation, FRotator::ZeroRotator);
        Bomb->LaunchInDirection(Directions[Counter] + FVector::UpVector);
        Bomb->SetWeaponDamage(WeaponDamage);
    }
}
```

The spawn location for the bombs is slightly above the player's head. They are spawned at the player's
left, right, forward, and backward directions. Only two vectors are needed and the other two are the same but
with a negative sign. All directions are assigned to an array.

The for loop iterates through the `Directions` array. First, a bomb is spawned, then the bomb calls its `LaunchInDirection`
function and sets its weapon damage. The `LaunchInDirection` function is defined as follows:

```cpp
// Bomb.cpp

void ABomb::LaunchInDirection(const FVector& Direction) const
{
    BombMesh->AddImpulse(Direction * 7500);
    FVector RotationDirection = Direction;
    RotationDirection.Z = 0;
    BombMesh->AddAngularImpulseInDegrees(RotationDirection * 750, NAME_None, true);
}
```

The `AddImpluse` function is used to "push" the bomb in a certain direction using the physics engine.
Then, a spin is applied with `AddAngularImpulseInDegrees` to make it look like the bomb was actually thrown.

## Credits

This project contains various third party assets.

See [Third Party Notice](ThirdPartyNotice.md).
