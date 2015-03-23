// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UsableActor.generated.h"

UCLASS()
class FPSTUTORIAL_API AUsableActor : public AStaticMeshActor
{
	GENERATED_UCLASS_BODY()

	

	// Sets default values for this actor's properties
	AUsableActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintImplementableEvent)
		bool OnUsed(ACharacter* character);

	UFUNCTION(BlueprintImplementableEvent)
		bool StartFocusItem();

	UFUNCTION(BlueprintImplementableEvent)
		bool EndFocusItem();
	
	
};
