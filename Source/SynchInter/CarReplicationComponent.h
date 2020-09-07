// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CarMovementComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Debug_Tool.h"
#include "CarReplicationComponent.generated.h"

USTRUCT()
struct FServerState
{
	GENERATED_BODY()

	UPROPERTY()
	FTransform Server_Transform;

	UPROPERTY()
	FVector Server_Velocity;

	UPROPERTY()
	FMoveData Server_LastMove;

	UPROPERTY()
	FVector Location;
};

struct FHermiteCubeSpline
{
	FVector StartLocation, StartDerivative, TargetLocation, TargetDerivative;

	FVector InterpolateLocation(float LerpRatio) const // Stop modification to struct
	{
		return FMath::CubicInterp(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}

	FVector InterpolateDerivative(float LerpRatio) const
	{
		return FMath::CubicInterpDerivative(StartLocation, StartDerivative, TargetLocation, TargetDerivative, LerpRatio);
	}
};




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNCHINTER_API UCarReplicationComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCarReplicationComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps)const;

	void ClientTick(float DeltaTime);

	void InterpolateLocation(FHermiteCubeSpline& Spline, float LerpRatio);

	void InterpolateVeloctiy(FHermiteCubeSpline& Spline, float LerpRatio);

	void InterpolateRotation(FHermiteCubeSpline& Spline, float LerpRatio);

	FHermiteCubeSpline CreateSpline();

	float GetVelocityToDerivitive() { return ClientTimeBetweenLastUpdates * 100;};

	float ClientTimeSinceUpdate;
	float ClientTimeBetweenLastUpdates;
	FTransform ClientStartTransform;
	FVector ClientStartVelocity;

private:
	UFUNCTION(Server, Reliable, WithValidation)// WithValidation, Reliable, NetMulticastr, Server, Client
	void ServerRPC_SendMove(FMoveData Move);
	
	UFUNCTION()// Replicated on state change
	void OnRep_ServerState();

	UPROPERTY(ReplicatedUsing = OnRep_ServerState)//Property Replication, replicating properties from client to server to client(on state change)
	FServerState ServerState;//transform

	UPROPERTY()
	UCarMovementComponent* MovementComponent;

	UFUNCTION()
	void ClearAcknowledgeMoves(FMoveData LastMove);

	void SimulatedProxy_OnRep_ServerState();
	void AutonomousProxy_OnRep_ServerState();
	void UpdateServerState(const FMoveData& LastMove);

	TArray<FMoveData> UnacknowledgedMoves;

	UPROPERTY()
	USceneComponent* MeshOffSetRoot;

	UFUNCTION(BlueprintCallable)
	void SetMeshOffSetRoot(USceneComponent* Root) {MeshOffSetRoot= Root;}
	float ClientSimulatedTime;
	UDebug_Tool* pr;
};


