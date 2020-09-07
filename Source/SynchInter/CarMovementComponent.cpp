// Fill out your copyright notice in the Description page of Project Settings.

#include "CarMovementComponent.h"
#include "Engine/World.h"

//Print shortcuts
#define prString(text)				pr->PrintString(text)
#define prVec(text)					pr->PrintVec(text)
#define prStr(text)					pr->PrintString(text)
#define prInt(text)					pr->PrintInt(text)
#define prFlt(text)					pr->PrintFloat(text)
#define prBool(text)				pr->PrintBool(text)
#define prActTrans(text)			pr->PrintActTrans(text)
#define prStrToInt(text)			pr->PrintStringToInt(text)
#define prFltWitStr(text,Flt)		pr->PrintFloatWitStr(text,Flt)
#define prVecWitStr(text,Vec)		pr->PrintVectorWitStr(text,Vec)
#define prActTraWitStr(text,Tra)	pr->PrintActTraWitStr(text,Tra)

// Sets default values for this component's properties
UCarMovementComponent::UCarMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	//SetIsReplicated(true);
}

// Called when the game starts
void UCarMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void UCarMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Authority - Server GetRemoteRole() == ROLE_SimulatedProxy// aganin changed from IsLocControled back to GetRemoteRole
	if (GetOwnerRole() == ROLE_AutonomousProxy || GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy) {
		//Basicly saying // We are the server and in control of the pawn./IsLocallyControlled()
		LastMove = CreateMove(DeltaTime);
		SimulateMove(LastMove);
	}
}

void UCarMovementComponent::SimulateMove(const FMoveData& Move)
{
	//Moving car forwards and backwards, velocity with physics implemented
	FVector Force = GetOwner()->GetActorForwardVector() * MaxDrivingForce * Move.Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();
	FVector Acceleration = Force / Mass;
	Velocity = Velocity + Acceleration * Move.DeltaTime;
	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);//Turning
	UpdateLocationFromVelocity(Move.DeltaTime);//Moving Forward
}

//StoreInputData
FMoveData UCarMovementComponent::CreateMove(float DeltaTime)
{
	// Save the last input from player to struct
	FMoveData InputData;
	InputData.Throttle = Throttle ;
	InputData.SteeringThrow = SteeringThrow;
	InputData.DeltaTime = DeltaTime ;//Delta Time
	InputData.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();//More efficient then GetWorld->TimeSeconds
	return InputData;
}


////////////////////////////////////////////////////////////// Physics for vehicle/sidan
FVector UCarMovementComponent::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector UCarMovementComponent::GetRollingResistance()
{
	// (F = m * g)  NormalForce = Mass * Acceleration due to gravity
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

void UCarMovementComponent::ApplyRotation(float DeltaTime, float Steering_Throw)
{
	float DeltaLocation = FVector::DotProduct(GetOwner()->GetActorForwardVector(), Velocity) * DeltaTime;
	float RotationAngle = DeltaLocation / MinTurningRadius * Steering_Throw;
	FQuat RotationDelta(GetOwner()->GetActorUpVector(), RotationAngle);
	Velocity = RotationDelta.RotateVector(Velocity);
	GetOwner()->AddActorWorldRotation(RotationDelta);
}

void UCarMovementComponent::UpdateLocationFromVelocity(float DeltaTime)
{
	//Update position
	FVector Translation = Velocity * 100 * DeltaTime;
	FHitResult HitResult;
	GetOwner()->AddActorWorldOffset(Translation, true, &HitResult);
	if (HitResult.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;
	}
}























































/*
FVector UCarMovementComponent::GetVelocity() {
	return Velocity;
}
*/

/*
void UCarMovementComponent::SetVelocity(FVector setVelocity) {
	Velocity = setVelocity;
}
*/

/*
FSidanMove UCarMovementComponent::CreateMove(float DeltaTime)
{
	// Save the last input from player to struct
	FMoveData InputData;
	InputData.Throttle = Throttle;
	InputData.SteeringThrow = SteeringThrow;
	InputData.DeltaTime = DeltaTime;//Delta Time
	InputData.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();//More efficient then GetWorld->TimeSeconds

	//Move.Time = GetWorld()->TimeSeconds;
	//GameState->GetServerWorldTimeSeconds();
	//prFlt(Move.Time);

	return InputData;
}

*/