// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

UCLASS()
class FPSTUTORIAL_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	// Sets default values for this character's properties
	AFPSCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	
	/** Get actor derived from UsableActor currently looked at by the player */
	class AUsableActor* GetUsableInView();

	/* Max distance to use/focus on actors. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float MaxUseDistance;

	/* True only in first frame when focused on new usable actor. */
	bool bHasNewFocus;

	/* Actor derived from UsableActor currently in center-view. */
	AUsableActor* FocusedUsableActor;

public:


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

		
		void OnFire(); // Fire projectile event
		void Fire_Projectile(FVector MuzzleLocation, FRotator MuzzleRotation); //Visually fires ( spawn projectile )

	//Tell Server That we wish to fire
	UFUNCTION(reliable, Server, WithValidation) //RPC That will be called on client, but executed on Server
		void Server_OnFire(FVector MuzzleLocation, FRotator MuzzleRotation);
	virtual void Server_OnFire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);
	virtual bool Server_OnFire_Validate(FVector MuzzleLocation, FRotator MuzzleRotation);

	//Calls Fire_Projectile on all clients. 
	UFUNCTION(reliable, NetMultiCast, WithValidation) //RPC That will be called on Server, but executed on Client
		void Client_OnFire(FVector MuzzleLocation, FRotator MuzzleRotation);
	virtual void Client_OnFire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation);
	virtual bool Client_OnFire_Validate(FVector MuzzleLocation, FRotator MuzzleRotation);



	// First person Camera, public property
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* FirstPersonCameraComponent;

	//Constructor for AFPSCharacter
	AFPSCharacter(const FObjectInitializer& ObjectInitializer);

	/** Pawn mesh: 1st person view ( arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		USkeletalMeshComponent* FirstPersonMesh;

	/* Weapon Mesh */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	//	UStaticMeshComponent* WeaponMesh;

	//Gun's muzzle's offset from the camera location
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;
	//Projectile class to spawn
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AFPSProjectile> ProjectileClass;

	/** Use the actor currently in view (if derived from UsableActor */
	UFUNCTION(BlueprintCallable, WithValidation, Server, Reliable, Category = PlayerAbility)
		virtual void Use();
	
	virtual void Use_Implementation();
	virtual bool Use_Validate();

};
