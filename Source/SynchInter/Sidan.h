// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/InputComponent.h"

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
	// Sets default values for this pawn's properties
	ASidan();

	//***
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	//UInputComponent* PlayerInputComponent;

	void MoveForward(float Value);

	void MoveRight(float Value);

	//***

//protected:
	// Called when the game starts or when spawned
	//virtual void BeginPlay() override;*******

private:
	//Giving the mass a metric ton weight 
	UPROPERTY(EditAnywhere)
	float Mass = 1000;

	// The force applied to the car when the throttle is fully down (N) Newtons
	UPROPERTY(EditAnywhere)
	float MaxDrivingForce = 10000;
	
	//Minimun truning radius at full lock of wheele in meters.
	UPROPERTY(EditAnywhere)
	float MinTurningRadius = 10;

	//Higher means more drag
	UPROPERTY(EditAnywhere)
	float DragCoefficient = 16;

	//Higher means more rolling resistance, wiki for more examples
	UPROPERTY(EditAnywhere)
	float RollingResistanceCoefficient = 0.015;

	FVector Velocity;

	float Throttle;

	float SteeringThrow;

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