// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSTutorial.h"
#include "FPSCharacter.h"
#include "FPSProjectile.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine){
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("FPSCharacter!"));
	}
	
}

// Constructor
AFPSCharacter::AFPSCharacter(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer){

	//Create a CameraComponent
	FirstPersonCameraComponent = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = CapsuleComponent;

	//Position camera a bit above the eyes
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 20.0f + BaseEyeHeight);
	//Allow pawn to control rotation
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FirstPersonMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetOnlyOwnerSee(true);         // only the owning player will see this mesh
	FirstPersonMesh->AttachParent = FirstPersonCameraComponent;
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

	// everyone but the owner can see the regular body mesh
	Mesh->SetOwnerNoSee(true);

}

// Called every frame
void AFPSCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	//Setup key bindings
	InputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);

	//Setup Mouse input
	InputComponent->BindAxis("Turn", this, &AFPSCharacter::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &AFPSCharacter::AddControllerPitchInput);

	//Jumping
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);

	//Shooting
	InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);
}

void AFPSCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking or falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.0f;
		}
		// add movement in that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AFPSCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AFPSCharacter::OnStartJump(){
	bPressedJump = true;
}
void AFPSCharacter::OnStopJump(){
	bPressedJump = false;
}


// Is called from Client
void AFPSCharacter::OnFire(){

	//Get camera transform
	FVector CameraLoc;
	FRotator CameraRot;
	GetActorEyesViewPoint(CameraLoc, CameraRot);

	// MuzzleOffset is in camera space, so transform it to world space before offsetting from camera to find the final muzzle pos
	FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
	FRotator const MuzzleRotation = CameraRot;
	//MuzzleRotation.Pitch += 10.0f; // skew the aim upwards a bit?...
	//if (GEngine){
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("MuzzleLocation: %s"), *MuzzleLocation.ToString());
	//}
	//UE_LOG(LogTemp, Warning, TEXT("MuzzleLocation: %s"), *MuzzleLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("MuzzleRotation: %s"), *MuzzleRotation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("MuzzleRotation: %s"), MuzzleRotation.ToString() );

	//Tell server that we fired our gun
	Server_OnFire(MuzzleLocation, MuzzleRotation);

	
}

void AFPSCharacter::Server_OnFire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation){

	//Check ammo etc, then tell clients to fire.
	// Should probably calculate Hit etc here, on server.
	Client_OnFire(MuzzleLocation, MuzzleRotation);	//Calls Fire_Projectile on all clients ( including server )

}

bool AFPSCharacter::Server_OnFire_Validate(FVector const MuzzleLocation, FRotator MuzzleRotation){
	return true;
}

//Called on client, from server.
void AFPSCharacter::Client_OnFire_Implementation(FVector MuzzleLocation, FRotator MuzzleRotation){
	Fire_Projectile(MuzzleLocation, MuzzleRotation);
}

// Spawns a Projectile and sets direction and speed.
void AFPSCharacter::Fire_Projectile(FVector MuzzleLocation, FRotator MuzzleRotation){
	// Try and fire a projectile
	if (ProjectileClass != NULL){
		UWorld* const World = GetWorld();
		if (World){
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			//Spawn the projectile at the muzle
			//Spawn projectile at Server
			AFPSProjectile* const Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			if (Projectile){

				//UE_LOG(LogTemp, Warning, TEXT("MuzzleLocation: %s"), *MuzzleLocation.ToString());
				//UE_LOG(LogTemp, Warning, TEXT("MuzzleRotation: %s"), *MuzzleRotation.ToString());

				//Find the launch direction
				FVector const LaunchDir = MuzzleRotation.Vector();
				Projectile->InitVelocity(LaunchDir);
			}
		}

	}
}

bool AFPSCharacter::Client_OnFire_Validate(FVector MuzzleLocation, FRotator MuzzleRotation){
	return true;
}