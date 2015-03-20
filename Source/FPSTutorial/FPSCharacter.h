// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

UCLASS()
class FPSTUTORIAL_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	//Handles moving forward / backward
	UFUNCTION()
		void MoveForward(float Val);
	//handle strafing
	UFUNCTION()
		void MoveRight(float Val);

	//Sets jump flag when key is pressed
	UFUNCTION()
		void OnStartJump();
	//Clears jump flag when key is released
	UFUNCTION()
		void OnStopJump();

	//Function to handle firing the projectile
	UFUNCTION()
		void OnFire();

	// First person Camera, public property
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FirstPersonCameraComponent;

	//Constructor for AFPSCharacter
	AFPSCharacter(const FObjectInitializer& ObjectInitializer);

	/** Pawn mesh: 1st person view ( arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FirstPersonMesh;
	//Gun's muzzle's offset from the camera location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;
	//Projectile class to spawn
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFPSProjectile> ProjectileClass;
};
