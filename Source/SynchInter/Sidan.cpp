// Fill out your copyright notice in the Description page of Project Settings.

#include "Sidan.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Math/Vector.h"

//Tutorials for vehicle phisics 76-83 udemy
// Sets default values
ASidan::ASidan(){
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASidan::BeginPlay(){	
	Super::BeginPlay();
	if(HasAuthority()) 
	{//Debug Method to make sure the configuration we use is a good choice, needing to do a good job!
		NetUpdateFrequency = 1;//The server is updating the clients once every second
	}
}

//Property Replication - Replicating
void ASidan::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps)const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASidan, ReplicatedTransform);
	DOREPLIFETIME(ASidan, Velocity);
	DOREPLIFETIME(ASidan, Throttle);
	DOREPLIFETIME(ASidan, SteeringThrow);
}

FString GetEnumText(ENetRole Role){
	switch (Role)
	{	
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "ROLE_SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "ROLE_AutonomousProxy";
	case ROLE_Authority:
		return "ROLE_Authority";
	default:
		return "ERROR";
	}
}

// Called every frame - tick updates...
void ASidan::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
	
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();
	
	FVector Acceleration = Force / Mass;
	
	Velocity = Velocity + Acceleration * DeltaTime;


	for (int i = 0; i < State.StateArray.length(); i++)
	{
		State.StateArray.Add(State.TheMove.Transform);
		State.StateArray.Add(State.TheMove.Velocity);
		State.StateArray.Add(State.TheMove.Throttle);
		State.StateArray.Add(State.TheMove.SteeringThrow);
		State.StateArray.Add(State.TheMove.Time);
	}


	ApplyRotation(DeltaTime);//Turning
	UpdateLocationFromVelocity(DeltaTime);//Moving Foward
	
	if(HasAuthority()){
		ReplicatedTransform = GetActorTransform();
	}

	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);
}

void ASidan::RecordMove()
{
	for (int i = 0; i < State.StateArray.length(); i++)
	{
		State.StateArray.Add(State.TheMove.Transform);
		State.StateArray.Add(State.TheMove.Velocity);
		State.StateArray.Add(State.TheMove.Throttle);
		State.StateArray.Add(State.TheMove.SteeringThrow);
		State.StateArray.Add(State.TheMove.Time);
	}
}


//Net work updates - updated when the state changes
void ASidan::OnRep_ReplicatedTransform()
{
	SetActorTransform(ReplicatedTransform);
}

void ASidan::ApplyRotation(float DeltaTime)
{
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
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;
}

FVector ASidan::GetRollingResistance(){
	// (F = m * g)  NormalForce = Mass * Acceleration due to gravity
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;
}

void ASidan::UpdateLocationFromVelocity(float DeltaTime){
	//Update position
	FVector Translation = Velocity * 100 * DeltaTime;
	FHitResult HitResult;
	AddActorWorldOffset(Translation, true, &HitResult);
	if (HitResult.IsValidBlockingHit()){
		Velocity = FVector::ZeroVector;// Zero Vector gives you 0,0,0
	}
}

// Called to bind functionality to input
void ASidan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);//Super, reason for this?
	PlayerInputComponent->BindAxis("MoveForward", this, &ASidan::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASidan::MoveRight);
}

void ASidan::MoveForward(float Value){
	Throttle = Value;
	ServerRPC_MoveForward(Value);
}

void ASidan::MoveRight(float Value){
	SteeringThrow = Value;
	ServerRPC_MoveRight(Value);
}

//RPC member functions...
bool ASidan::ServerRPC_MoveForward_Validate(float Value){
	//The client could use double input or somthing of that nature, change input values in a .ini etc (*2)
	//if (Speed(Value) is gradual build up(0 - 10 in 10 secs)
	//if (Value >= 0 && Value <= 10) {
	return FMath::Abs(Value) <= 1;//Absolute value - number line  -2 to 0 to 2 = absolute value of (4)
}

void ASidan::ServerRPC_MoveForward_Implementation(float Value){
	Throttle = Value;
}

bool ASidan::ServerRPC_MoveRight_Validate(float Value){
	return FMath::Abs(Value) <= 1;
}

void ASidan::ServerRPC_MoveRight_Implementation(float Value){
	SteeringThrow = Value;
}
























/*
//RPC member functions...
// Called every frame - tick updates...
void ASidan::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	//Force to make mass accelerate, this leaves us with the accelerated mass in motion with velocity.
	//Force = mass * acceleration ( F = m * a ) or (a = F / m)
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();
	//Force = 1000 * acceleration
	//Acceleration = Force / mass   (a = F / m)
	//Mass is set at 1000 to represent a ton or 1000 kilos
	FVector Acceleration = Force / Mass;
	//float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;

	//Veloctiy
	Velocity = Velocity + Acceleration * DeltaTime;
	ApplyRotation(DeltaTime);
	UpdateLocationFromVelocity(DeltaTime);
	//GetEnumText(Role)

	//Replication - Server replicating on clients
	if (HasAuthority()) {
		ReplicatedTransform = GetActorTransform();
	}

	//else //{ //SetActorLocation(ReplicatedTransform); //SetActorRotation(ReplicatedRotation); //}

	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);
}
*/



/*
//Net work updates - updated when the state changes
void ASidan::OnRep_ReplicatedTransform()
{
	SetActorTransform(ReplicatedTransform);

	FString stringLocation = ReplicatedTransform.ToString();
	UE_LOG(LogTemp, Warning, TEXT("On_Rep Replicated Position %s"), *stringLocation);

	//SetActorRotation(ReplicatedRotation);
	//FString stringRotation = ReplicatedRotation.ToString();
	//UE_LOG(LogTemp, Warning, TEXT("On_Rep Replicated Position"));
}
*/