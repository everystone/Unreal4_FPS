// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSTutorial.h"
#include "FPSProjectile.h"


// Sets default values
AFPSProjectile::AFPSProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//Constructor
AFPSProjectile::AFPSProjectile(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer){

	//Use sphere as a simple collision representation
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(15.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile"); //use our Projectile collision stuff
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProjectile::OnHit);
	RootComponent = CollisionComp;

	//Use a ProjectileMovementcomponent to handle this projectile's movement
	ProjectileMovement = ObjectInitializer.CreateDefaultSubobject<UProjectileMovementComponent>(this, TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 2000.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;
	ProjectileMovement->Bounciness = 0.3f;

	InitialLifeSpan = 5.0f; // Die after 5 seconds
}

void AFPSProjectile::InitVelocity(const FVector& ShootDirection){
	if (ProjectileMovement){
		//Set the projectile's velocity to the desired direction
		ProjectileMovement->Velocity = ShootDirection * ProjectileMovement->InitialSpeed;
		UE_LOG(LogTemp, Warning, TEXT("Velocity OK"));
	}
}


void AFPSProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	if (OtherActor && (OtherActor != this) && OtherComp){
		
		//Adds a physics impulse to Whatever the projectile hits
		if (OtherComp->Mobility > 0 && OtherComp->IsSimulatingPhysics())
		OtherComp->AddImpulseAtLocation(ProjectileMovement->Velocity * 100.0f, Hit.ImpactPoint);
	}
}