// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSTutorial.h"
#include "UsableActor.h"



// Called when the game starts or when spawned
void AUsableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUsableActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//Constructor
AUsableActor::AUsableActor(const class FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer){

}


