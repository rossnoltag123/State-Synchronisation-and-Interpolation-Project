// Fill out your copyright notice in the Description page of Project Settings.

//#include "Components/InputComponent.h"
#include "Sidan.h"
//#include "Components/InputComponent.h"
#include "Engine/World.h"
//Tutorials for vehicle phisics 76-83 udemy
// Sets default values
ASidan::ASidan(){
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASidan::BeginPlay(){	
	Super::BeginPlay();
}

// Called every frame
void ASidan::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	//Force = mass * acceleration ( F = m * a ) or (a = F / m)
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();
	//Force = 1000 * acceleration
	//Acceleration = Force / mass   (a = F / m)
	//Mass is set at 1000 to represent a ton or 1000 kilos
	FVector Acceleration = Force / Mass;

	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	//Veloctiy
	Velocity = Velocity + Acceleration * DeltaTime;
	ApplyRotation(DeltaTime);
	UpdateLocationFromVelocity(DeltaTime);
}

void ASidan::ApplyRotation(float DeltaTime){
	//DotProduct = What portion of the Velocity is in the forward direction
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(),Velocity) * DeltaTime;

	//Rotation Angle
	float RotationAngle = DeltaLocation / MinTurningRadius * SteeringThrow;

	//Rotations on an axil - in this case rotations on the axis
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);

	Velocity = RotationDelta.RotateVector(Velocity);

	AddActorWorldRotation(RotationDelta);
}

FVector ASidan::GetAirResistance(){
	return - Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector ASidan::GetRollingResistance(){
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return - Velocity.GetSafeNormal()* RollingResistanceCoefficient * NormalForce;
}

void ASidan::UpdateLocationFromVelocity(float DeltaTime){
	//Update position
	FVector Translation = Velocity * 100 * DeltaTime;
	FHitResult HitResult;
	AddActorWorldOffset(Translation, true, &HitResult);

	if (HitResult.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;// Zero Vector gives you 0,0,0
	}
}


// Called to bind functionality to input
void ASidan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASidan::MoveForward);
	//PlayerInputComponent->BindAxis("MoveBack", this, &ASidan::MoveForward);
	//PlayerInputComponent->BindAxis("MoveLeft", this, &ASidan::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASidan::MoveRight);
}

void ASidan::MoveForward(float Value)
{
	//Vecvtors - Get direction of car , forward vector xyz.
	Throttle = Value;
}

void ASidan::MoveRight(float Value)
{
	//Vecvtors - Get direction of car , forward vector xyz.
	SteeringThrow = Value;
}