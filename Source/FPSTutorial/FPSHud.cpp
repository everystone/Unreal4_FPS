// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSTutorial.h"
#include "FPSHud.h"



//Constructor
AFPSHud::AFPSHud(const FObjectInitializer& ObjectInitializer)
:Super(ObjectInitializer){

	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/hud/crosshair.crosshair'"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AFPSHud::DrawHUD(){
	Super::DrawHUD();

	//Draw simple crosshair
	//find center of canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	//Offset by half texture dimension
	const FVector2D CrosshairDrawPosition((Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5f)), (Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5f)));

	//Draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}