// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Math/Vector.h"
#include "Components/InputComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Sidan.generated.h"

//Canonical(a sequence of moves) state, sending the moves in one type "FMove", to make sure the recoderd moves are sent in one packet
USTRUCT()
struct FMove
{
	GENERATED_BODY()

	FTransform Transform;
	FVector Velocity;
	float Throttle;
	float SteeringThrow;
	float Time;//Delta Time
};

USTRUCT()
struct FCanonicalState
{
	GENERATED_BODY()

	FMove TheMove;
	TArray<TheMove> StateArray;
	
};


UCLASS()
class SYNCHINTER_API ASidan : public APawn
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Sets default values for this pawn's properties
	ASidan();
	
	//***
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//UInputComponent* PlayerInputComponent;
	//void MoveForward(float Value);
	//void MoveRight(float Value);
	//***

//protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;*******

private: 
	void MoveForward(float Value);// AP Autonomnous proxy local controlls
	void MoveRight(float Value);

private:
	UFUNCTION(Server, Reliable, WithValidation)// WithValidation, Reliable, NetMulticastr, Server, Client
	void ServerRPC_MoveForward(float Value);

	UFUNCTION(Server, Reliable, WithValidation)// WithValidation, Reliable, NetMulticastr, Server, Client
	void ServerRPC_MoveRight(float Value); //No headers necessary,methods in cpp file must be named "ServerRPC...then method"
	

	void RecordMove();

private:
	//Giving the mass a metric ton weight 
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// The force applied to the car when the throttle is fully down 
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;//(Air Resistance) Default driving force 10000/(Mass)1000 = 10 acceleration (10 Meters per second)
	
	//Minimun truning radius at full lock of wheele in meters.
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	//Higher means more drag
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16; // (MaxDrivingForce)AirResistance/Speed(Sqrdby)2 = DragCoefficient

	//Higher means more rolling resistance, wiki for more examples
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;


public:
	//UPROPERTY(ReplicatedUsing = OnRep_ReplicatedTransform)//Property Replication, replicating a properties from the server
	//FVector ReplicatedRotation;

	//UFUNCTION()
	//void OnRep_ReplicatedRotation();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps)const;

	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedTransform)//Property Replication, replicating properties from client to server to client(on state change)
	FTransform ReplicatedTransform;//transform

	UFUNCTION()// Replicated on state change
	void OnRep_ReplicatedTransform();

	//UPROPERTY(Replicated)
	//FVector Velocity;

	//UPROPERTY(Replicated)
	//float Throttle;

	//UPROPERTY(Replicated)
	//float SteeringThrow;

	UPROPERTY(Replicated)
	FCanonicalState State;

private:   
	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime);
	FVector GetAirResistance();
	FVector GetRollingResistance();
};

































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