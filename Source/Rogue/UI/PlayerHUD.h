#pragma once

#include "CoreMinimal.h"
#include "Engine/Canvas.h"
#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 *
 */
UCLASS()
class ROGUE_API APlayerHUD : public AHUD
{
    GENERATED_BODY()

  protected:
    UPROPERTY(EditDefaultsOnly)
    UTexture2D* CrosshairTexture;

  public:
    virtual void DrawHUD() override;
};
