// Fill out your copyright notice in the Description page of Project Settings.

#include "Sidan.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "DrawDebugHelpers.h"

//using namespace UCarMovementComponent;

//Print shortcuts
#define prString(text)				pr->PrintString(text)
#define prVec(text)					pr->PrintVec(text)
#define prStr(text)					pr->PrintString(text)
#define prInt(text)					pr->PrintInt(text)
#define prFlt(text)					pr->PrintFloat(text)
#define prBool(text)				pr->PrintBool(text)
#define prActTrans(text)			pr->PrintActTrans(text)
#define prStrToInt(text)			pr->PrintStringToInt(text)
#define prInt32WitStr(text,Int)		pr->PrintInt32WitStr(text, Int)
#define prFltWitStr(text,Flt)		pr->PrintFloatWitStr(text,Flt)
#define prVecWitStr(text,Vec)		pr->PrintVectorWitStr(text,Vec)
#define prActTraWitStr(text,Tra)	pr->PrintActTraWitStr(text,Tra)

//Tutorials for vehicle physics 76-83 udemy
// Sets default values
ASidan::ASidan()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bReplicateMovement = false;

	MovementComponent = CreateDefaultSubobject<UCarMovementComponent>(TEXT("Car Movement Component"));
	ReplicationComponent = CreateDefaultSubobject<UCarReplicationComponent>(TEXT("Car Replication Component"));
}

// Called when the game starts or when spawned
void ASidan::BeginPlay()
{	
	Super::BeginPlay();
	if(HasAuthority()) 
	{//Debug Method to make sure the configuration we use is a good choice, needing to do a good job!
		NetUpdateFrequency = 1;//The server is updating the clients once every second
	}
}

FString GetEnumText(ENetRole Role)
{
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
void ASidan::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);
}

//////////////////////////////////////////////////////////Called to bind functionality to input
void ASidan::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);//Super, reason for this?
	PlayerInputComponent->BindAxis("MoveForward", this, &ASidan::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASidan::MoveRight);
}

void ASidan::MoveForward(float Value) { MovementComponent->SetThrottle(Value);}// Autonomous move forward//Dont access private variables, get set instead

void ASidan::MoveRight(float Value) { MovementComponent->SetSteeringThrow(Value);}// Autonomous move forward


































/*
		if (MovementComponent == nullptr) return;

		// Autonomous
		if (Role == ROLE_AutonomousProxy){
			FMoveData Movedata = MovementComponent->CreateMove(DeltaTime);
			MovementComponent->SimulateMove(Movedata);
			UnacknowledgedMoves.Add(Movedata);
			ServerRPC_SendMove(Movedata);
		}

		// Authority - Server GetRemoteRole() == ROLE_SimulatedProxy
		if (Role == ROLE_Authority && IsLocallyControlled()){
			//Basicly saying // We are the server and in control of the pawn./IsLocallyControlled()
			FMoveData Movedata = MovementComponent->CreateMove(DeltaTime);
			ServerRPC_SendMove(Movedata);
		}

		// Simulated Proxy
		if (Role == ROLE_SimulatedProxy){
			MovementComponent->SimulateMove(ServerState.Server_LastMove);
		}
*/

/*
void ASidan::ClearAcknowledgeMoves(FMoveData LastMove)
{
	TArray<FMoveData> NewMoves;
	for (const FMoveData& Move : UnacknowledgedMoves)
	{
		if (Move.Time > LastMove.Time)
		{
			NewMoves.Add(Move);
		}
	}
	UnacknowledgedMoves = NewMoves;
}
*/














/*
//Property Replication - Replicating
void ASidan::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps)const{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ASidan, ServerState);
}
*/




/*
void ASidan::OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;

	//Set transform on client from server
	SetActorTransform(ServerState.Server_Transform);
	MovementComponent->SetVelocity(ServerState.Server_Velocity);//*************************************************************************
	ClearUnacknowledgedMoves(ServerState.Server_LastMove);

	for (const FMoveData& Mv : UnacknowledgedMoves){
		MovementComponent->SimulateMove(Mv);
	}
}

//RPC member functions...
bool ASidan::ServerRPC_SendMove_Validate(FMoveData Move)
{
	//The client could use double input or somthing of that nature, change input values in a .ini etc (*2)
	//if (Speed(Value) is gradual build up(0 - 10 in 10 secs)
	//if (Value >= 0 && Value <= 10) {
	return true;//Absolute value - number line  -2 to 0 to 2 = absolute value of (4)
}

//On the server...
void ASidan::ServerRPC_SendMove_Implementation(FMoveData Move)
{
	if (MovementComponent == nullptr) return;
	
	//Simulate Move on server
	MovementComponent->SimulateMove(Move);
	
	//Set last move, will be used for Unacknowledged moves queue later.
	ServerState.Server_LastMove = Move;
	
	//Set ServerState
	ServerState.Server_Transform = GetActorTransform();
	ServerState.Server_Velocity = MovementComponent->GetVelocity();
	prFltWitStr("SteeringThrow...............Implement", Move.SteeringThrow);
}
*/































































































//UCarMovementComponent* Move = CreateMove(DeltaTime);
//	FMoveData Movedata = MovementComponent->CreateMove(DeltaTime);

	//Massive interuption possablity, Adding to the queue from Autonomous and Authority  
	// No need to adsd to server, the move queue is for the server
//	if (!HasAuthority()) //!HasAuthority is equivilent to saying it is the autonomous proxy
//	{
		//Moved SimulateMove here because it was being called twice when in the server
	//	UnacknowledgedMoves.Add(Movedata);
	//	MovementComponent->SimulateMove(Movedata);
	//	prInt32WitStr("Queue Length: ",UnacknowledgedMoves.Num());
//	}

	//Create Move and send it Sending Move
	//ServerRPC_SendMove(Movedata);//Not sending values from here...Try one at time














/////////////////////////////////////////////////////////Server to client updates - updated when the state changes
/*
FMoveData UCarMovementComponent::CreateMove(float DeltaTime)
{
	// Save the last input from player to struct
	FMoveData Move;
	Move.Throttle = Throttle;
	Move.SteeringThrow = SteeringThrow;
	Move.DeltaTime = DeltaTime;//Delta Time
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();//More efficient then GetWorld->TimeSeconds
	return Move;
}
*/

/*
////////////////////////////////////////////////////////////// Physics for vehicle/sidan
void ASidan::SimulateMove(const FSidanMove& Move)
{
	//if (IsLocallyControlled())
	//{
		//prVecWitStr("Velocity in Simulate move",Velocity);//**************************************************************
	//}

	//Moving car forwards and backwards, velocity with phiysics implemented
	FVector Force = GetActorForwardVector() * MaxDrivingForce * Move.Throttle;
	Force += GetAirResistance();
	Force += GetRollingResistance();
	FVector Acceleration = Force / Mass;
	Velocity = Velocity + Acceleration * Move.DeltaTime;	//************************************************************
	if (IsLocallyControlled())
	{
		prVecWitStr("Velocity in Simulate move", Velocity);//**************************************************************
	}
	ApplyRotation(Move.DeltaTime, Move.SteeringThrow);//Turning
	UpdateLocationFromVelocity(Move.DeltaTime);//Moving Foward
}

FVector ASidan::GetAirResistance()
{
	return -Velocity.GetSafeNormal() * Velocity.SizeSquared() * DragCoefficient;//****************************************
}

FVector ASidan::GetRollingResistance()
{
	// (F = m * g)  NormalForce = Mass * Acceleration due to gravity
	float AccelerationDueToGravity = -GetWorld()->GetGravityZ() / 100;
	float NormalForce = Mass * AccelerationDueToGravity;
	return -Velocity.GetSafeNormal() * RollingResistanceCoefficient * NormalForce;//**************************************
}

void ASidan::ApplyRotation(float DeltaTime, float Steering_Throw) 
{
	float DeltaLocation = FVector::DotProduct(GetActorForwardVector(), Velocity) * DeltaTime;//****************************
	float RotationAngle = DeltaLocation / MinTurningRadius * Steering_Throw;
	FQuat RotationDelta(GetActorUpVector(), RotationAngle);
	Velocity = RotationDelta.RotateVector(Velocity);//*********************************************************************
	AddActorWorldRotation(RotationDelta);
}

void ASidan::UpdateLocationFromVelocity(float DeltaTime)
{
	//Update position
	FVector Translation = Velocity * 100 * DeltaTime;//*********************************************************************
	FHitResult HitResult;
	AddActorWorldOffset(Translation, true, &HitResult);
	if (HitResult.IsValidBlockingHit())
	{
		Velocity = FVector::ZeroVector;// Zero Vector gives you 0,0,0//*****************************************************
	}
}
*/
// Garbage Collection, Pointers and References, virtual funtions,

/*
FSidanMove ASidan::CreateMove(float DeltaTime)
{
	// Save the last input from player to struct
	FSidanMove Move;
	Move.Throttle = Throttle;
	Move.SteeringThrow = SteeringThrow;
	Move.DeltaTime = DeltaTime;//Delta Time
	Move.Time = GetWorld()->GetGameState()->GetServerWorldTimeSeconds();//More efficient then GetWorld->TimeSeconds

	//Move.Time = GetWorld()->TimeSeconds;
	//GameState->GetServerWorldTimeSeconds();
	//prFlt(Move.Time);

	return Move;
}
*/

/*
bool ASidan::ServerRPC_SendMove_Validate(float Value){
	return FMath::Abs(Value) <= 1;
}

void ASidan::ServerRPC_MoveRight_Implementation(float Value){
	SteeringThrow = Value;
}
*/

/*
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
*/

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