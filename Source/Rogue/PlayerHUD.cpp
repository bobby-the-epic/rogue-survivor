#include "PlayerHUD.h"

void APlayerHUD::DrawHUD()
{
    Super::DrawHUD();

    FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
    FVector2D CrosshairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth() * 0.5f),
                                    Center.Y - (CrosshairTexture->GetSurfaceHeight() * 0.5f));
    FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTexture->GetResource(), FLinearColor::White);
    TileItem.BlendMode = ESimpleElementBlendMode::SE_BLEND_Translucent;
    Canvas->DrawItem(TileItem);
}
