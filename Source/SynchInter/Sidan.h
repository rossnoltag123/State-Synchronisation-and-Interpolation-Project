// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CarMovementComponent.h"
#include "CarReplicationComponent.h"
#include "Debug_Tool.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Sidan.generated.h"

UCLASS()
class SYNCHINTER_API ASidan : public APawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ASidan();

	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCarMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)//Not allowed to re point, Edit Anywhere - allowed to re point
	UCarReplicationComponent* ReplicationComponent;

private: 
	void MoveForward(float Value);// AP Autonomous proxy local controls
	void MoveRight(float Value);

	UDebug_Tool* pr;
};














































//TArray<FMoveData> UnacknowledgedMoves;

//UFUNCTION()
//void ClearAcknowledgeMoves(FMoveData LastMove);

/*
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
};
*/

/*

//PLAYER INPUT*************************************************************************************************************
	// Called to bind functionality to input
virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void MoveForward(float Value);// AP Autonomnous proxy local controlls
	void MoveRight(float Value);
	/*
	//RPC'S**********************************************************************************************************************
	public:
		void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps)const;

	private:
		UFUNCTION(Server, Reliable, WithValidation)// WithValidation, Reliable, NetMulticastr, Server, Client
		void ServerRPC_SendMove(FMoveData Move);

	//REPLICATED******************************************************************************************************************
		UFUNCTION()// Replicated on state change
		void OnRep_ServerState();

		UPROPERTY(ReplicatedUsing = OnRep_ServerState)//Property Replication, replicating properties from client to server to client(on state change)
		FServerState ServerState;//transform
*/


//UInputComponent* PlayerInputComponent;
//void MoveForward(float Value);
//void MoveRight(float Value);
//***


//UFUNCTION(Server, Reliable, WithValidation)// WithValidation, Reliable, NetMulticastr, Server, Client
//void ServerRPC_MoveRight(float Value); //No headers necessary,methods in cpp file must be named "ServerRPC...then method"



//UPROPERTY(ReplicatedUsing = OnRep_ReplicatedTransform)//Property Replication, replicating a properties from the server
//FVector ReplicatedRotation;

//UFUNCTION()
//void OnRep_ReplicatedRotation();

//public:
	// Called every frame
	//virtual void Tick(float DeltaTime) override;*******************

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;**************

	//UInputComponent* PlayerInputComponent;

	//void MoveForward(float Value);********
	//void MoveRight(float Value);***********


//protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;*******