// Fill out your copyright notice in the Description page of Project Settings.

#include "CarReplicationComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
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
#define prInt32WitStr(text,Int)		pr->PrintInt32WitStr(text, Int)
#define prFltWitStr(text,Flt)		pr->PrintFloatWitStr(text,Flt)
#define prVecWitStr(text,Vec)		pr->PrintVectorWitStr(text,Vec)
#define prActTraWitStr(text,Tra)	pr->PrintActTraWitStr(text,Tra)

UCarReplicationComponent::UCarReplicationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicated(true);
}

void UCarReplicationComponent::BeginPlay()
{
	Super::BeginPlay();
	MovementComponent = GetOwner()->FindComponentByClass<UCarMovementComponent>();
}

// Called every frame
void UCarReplicationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction){
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (MovementComponent == nullptr) return;
	FMoveData LastMove = MovementComponent->GetLastMove();

	// Autonomous
	if (GetOwnerRole() == ROLE_AutonomousProxy)
	{
		UnacknowledgedMoves.Add(LastMove);
		ServerRPC_SendMove(LastMove);
	}

	// Authority GetOwner()->GetRemoteRole()//GetOwnerRole() == ROLE_Authority &&//GetOwnerRole() == ROLE_Authority &&  
	if (GetOwner()->GetRemoteRole() == ROLE_SimulatedProxy)
	{
		UpdateServerState(LastMove);
	}

	 //Simulated Proxy
	if (GetOwnerRole() == ROLE_SimulatedProxy)
	{
		//MovementComponent->SimulateMove(ServerState.Server_LastMove);
		ClientTick(DeltaTime);
	}
}

// Updated on state change
void UCarReplicationComponent::OnRep_ServerState()
{
	switch (GetOwnerRole())
	{
	case ROLE_SimulatedProxy:
		return SimulatedProxy_OnRep_ServerState();
		break;
	case ROLE_AutonomousProxy:
		return AutonomousProxy_OnRep_ServerState();
		break;
	default:
		break;
	}
}

void UCarReplicationComponent::ClientTick(float DeltaTime)
{
	ClientTimeSinceUpdate += DeltaTime;
	if (ClientTimeBetweenLastUpdates < KINDA_SMALL_NUMBER) return;
	if (MovementComponent == nullptr) return;

	float LerpRatio = ClientTimeSinceUpdate / ClientTimeBetweenLastUpdates;
	// float VelocityToDerivitive = ClientTimeBetweenLastUpdates * 100;
	// Hermite Cubic Spline Interpolation - Cubic Interpolation 
	//Conversion - (Velocity in Meters Per Second, Location in Cm's)
	FHermiteCubeSpline Spline = CreateSpline();

	//FVector NewLocation = Spline.InterpolateLocation(LerpRatio);
	//FVector NewDerivitive = Spline.InterpolateDerivative(LerpRatio);
	//FVector NewVeloctiy = NewDerivitive / GetVelocityToDerivitive();

	// LERP FVector NewLocation = FMath::LerpStable(StartLocation, TargetLocation, LerpRatio);
	//Rotation - Slerp
	//FQuat TargetRotation = ServerState.Server_Transform.GetRotation();
	//FQuat StartRotation = ClientStartTransform.GetRotation();
	//FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);

	// MovementComponent->SetVelocity(NewVeloctiy);
	// GetOwner()->SetActorLocation(NewLocation);
	//GetOwner()->SetActorRotation(NewRotation);

	InterpolateLocation(Spline, LerpRatio);
	InterpolateVeloctiy(Spline, LerpRatio);
	InterpolateRotation(Spline, LerpRatio);
}

void UCarReplicationComponent::InterpolateLocation(FHermiteCubeSpline& Spline, float LerpRatio) {

	FVector NewLocation = Spline.InterpolateLocation(LerpRatio);
	if (MeshOffSetRoot != nullptr)
	{
		MeshOffSetRoot->SetWorldLocation(NewLocation);
	}
}

void UCarReplicationComponent::InterpolateVeloctiy(FHermiteCubeSpline& Spline, float LerpRatio) {

	if (MovementComponent == nullptr) return;
	FVector NewDerivitive = Spline.InterpolateDerivative(LerpRatio);
	FVector NewVeloctiy = NewDerivitive / GetVelocityToDerivitive();
	MovementComponent->SetVelocity(NewVeloctiy);
}

void UCarReplicationComponent::InterpolateRotation(FHermiteCubeSpline& Spline, float LerpRatio) {

	FQuat TargetRotation = ServerState.Server_Transform.GetRotation();
	FQuat StartRotation = ClientStartTransform.GetRotation();
	FQuat NewRotation = FQuat::Slerp(StartRotation, TargetRotation, LerpRatio);

	if (MeshOffSetRoot != nullptr)
	{
		MeshOffSetRoot->SetWorldRotation(NewRotation);
	}
}

FHermiteCubeSpline UCarReplicationComponent::CreateSpline()
{
	FHermiteCubeSpline Spline;
	Spline.TargetLocation = ServerState.Server_Transform.GetLocation();
	Spline.StartLocation = ClientStartTransform.GetLocation();
	Spline.StartDerivative = ClientStartVelocity * GetVelocityToDerivitive();
	Spline.TargetDerivative = ServerState.Server_Velocity * GetVelocityToDerivitive();
	return Spline;
}

void UCarReplicationComponent::AutonomousProxy_OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;
	GetOwner()->SetActorTransform(ServerState.Server_Transform);
	MovementComponent->SetVelocity(ServerState.Server_Velocity);
	ClearAcknowledgeMoves(ServerState.Server_LastMove);

	for (const FMoveData& Mv : UnacknowledgedMoves)
	{
		MovementComponent->SimulateMove(Mv);
	}
}

void UCarReplicationComponent::SimulatedProxy_OnRep_ServerState()
{
	if (MovementComponent == nullptr) return;
	ClientTimeBetweenLastUpdates = ClientTimeSinceUpdate;
	ClientTimeSinceUpdate = 0;

	if (MeshOffSetRoot != nullptr) {
		ClientStartTransform.SetLocation(MeshOffSetRoot->GetComponentLocation());
		ClientStartTransform.SetRotation(MeshOffSetRoot->GetComponentQuat());
	}

	ClientStartVelocity = MovementComponent->GetVelocity();
	GetOwner()->SetActorTransform(ServerState.Server_Transform);
}

void UCarReplicationComponent::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCarReplicationComponent, ServerState);
}

bool UCarReplicationComponent::ServerRPC_SendMove_Validate(FMoveData Move)
{
	//The client could use double input or somthing of that nature, change input values in a .ini etc (*2)
	//if (Speed(Value) is gradual build up(0 - 10 in 10 secs)
	//if (Value >= 0 && Value <= 10) {

	float ProposedTime = ClientSimulatedTime + Move.DeltaTime;
	bool ClientNotRunningAhead = ProposedTime < GetWorld()->TimeSeconds;

	if (!ClientNotRunningAhead) {
		UE_LOG(LogTemp, Error, TEXT("Client is running to fast"));
		return false;
	}

	if (!Move.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Received invalid move."))
			return false;
	}

	return true;//Absolute value - number line  -2 to 0 to 2 = absolute value of (4)
}

void UCarReplicationComponent::ServerRPC_SendMove_Implementation(FMoveData Move)
{
	if (MovementComponent == nullptr) return;

	ClientSimulatedTime += Move.DeltaTime;

	MovementComponent->SimulateMove(Move);
	UpdateServerState(Move);
}

void UCarReplicationComponent::UpdateServerState(const FMoveData& LastMove)
{
	//Set last move, will be used for Unacknowledged moves queue later
	ServerState.Server_LastMove = LastMove;
	ServerState.Server_Transform = GetOwner()->GetActorTransform();
	ServerState.Server_Velocity = MovementComponent->GetVelocity();
}

void UCarReplicationComponent::ClearAcknowledgeMoves(FMoveData LastMove)
{
	TArray<FMoveData> NewMoves;
	for (const FMoveData& UnacknowledgedMove : UnacknowledgedMoves)
	{
		if (UnacknowledgedMove.Time > LastMove.Time) {NewMoves.Add(UnacknowledgedMove);}
	}
	UnacknowledgedMoves = NewMoves;
}






























































//void UCarReplicationComponent::OnRep_ServerState() {
	/*
		if (MovementComponent == nullptr) return;
		GetOwner()->SetActorTransform(ServerState.Server_Transform);
		MovementComponent->SetVelocity(ServerState.Server_Velocity);
		ClearAcknowledgeMoves(ServerState.Server_LastMove);
		for(const FMoveData& Mv : UnacknowledgedMoves){
			MovementComponent->SimulateMove(Mv);
		}
	*/
//}