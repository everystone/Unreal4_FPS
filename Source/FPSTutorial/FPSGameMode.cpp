// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSTutorial.h"
#include "FPSGameMode.h"
//#include "FPSCharacter.h"
#include "FPSHud.h"

/* Constructor */
AFPSGameMode::AFPSGameMode(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer){

	//DefaultPawnClass = AFPSCharacter::StaticClass(); //Tells gamemode which type of pawn to spawn for player when starting game
	//Set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("Pawn'/Game/Blueprints/BP_FPSCharacter.BP_FPSCharacter_C'"));
	//To get this path, right click blueprint and select 'Copy reference'
	if (PlayerPawnObject.Class != NULL){
		DefaultPawnClass = PlayerPawnObject.Class;
	}

	//Setup our custom hud, with crosshair
	HUDClass = AFPSHud::StaticClass();
}

void AFPSGameMode::StartPlay()
{
	Super::StartPlay();

	StartMatch();

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("HELLO WORLDS! "));
	}
}