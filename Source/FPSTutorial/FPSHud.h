// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "FPSHud.generated.h"

/**
 * 
 */
UCLASS()
class FPSTUTORIAL_API AFPSHud : public AHUD
{
	GENERATED_BODY()
	
private:
	//Crosshair asset pointer
	UTexture2D* CrosshairTex;

	//Constr
	AFPSHud(const FObjectInitializer& ObjectInitializer);

	/* Override DrawHUD to draw our custom crosshair*/
	virtual void DrawHUD() override;
	
};
