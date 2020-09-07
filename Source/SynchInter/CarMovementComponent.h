// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Debug_Tool.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/GameStateBase.h"
#include "CarMovementComponent.generated.h"

//Canonical(a sequence of moves) state, sending the moves in one type "FMove",
//to make sure the recorded moves are sent in one packet

USTRUCT()//Private-Public
struct FMoveData
{
	// Stuct Data- arranged in memory side by side, instead of scattered throughout
	GENERATED_BODY()
	//A composite data type
	UPROPERTY()
	float Throttle;

	UPROPERTY()
	float SteeringThrow;

	UPROPERTY()
	float DeltaTime;//Delta Time

	UPROPERTY()
	float Time;

	bool IsValid() const 
	{
		return FMath::Abs(Throttle) <= 1 && FMath::Abs(SteeringThrow) <= 1;
	}
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SYNCHINTER_API UCarMovementComponent : public UActorComponent 
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCarMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	UFUNCTION()
	void SimulateMove(const FMoveData& Move);

	UFUNCTION()
	FMoveData CreateMove(float DeltaTime);

	FVector GetVelocity() { return Velocity; };
	void SetVelocity(FVector Value) { Velocity = Value; };
	
	void SetThrottle(float Value) { Throttle = Value; };
	void SetSteeringThrow(float Value) { SteeringThrow = Value; };

	FMoveData GetLastMove() { return LastMove; };

private:
	FVector Velocity;
	float Throttle;
	float SteeringThrow;

	FMoveData LastMove;

	void UpdateLocationFromVelocity(float DeltaTime);
	void ApplyRotation(float DeltaTime, float SteeringThrow);
	FVector GetAirResistance();
	FVector GetRollingResistance();

	AGameStateBase* GameState;

	//Giving the mass a metric ton weight 
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	//The force applied to the car when the throttle is fully down 
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;//(Air Resistance) Default driving force 10000/(Mass)1000 = 10 acceleration (10 Meters per second)

	//Minimum turning radius at full lock of wheel in meters.
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	//Higher means more drag
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16; //(MaxDrivingForce)AirResistance/Speed(Sqrdby)2 = DragCoefficient

	//Higher means more rolling resistance, wiki for more examples
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;

	//DEBUG SHORT CUTS********************************************************************************************************************************************
	UDebug_Tool* pr;

	// The prediction..
	// The prediction..
	// We haven received an update to say there is a turn,
	// The simulated proxies are yet to receive the update of the turn, while the authority or autonomous turn
	// New bug: car is shooting forward- looks like multiple move are being played and then being adjusted
};

