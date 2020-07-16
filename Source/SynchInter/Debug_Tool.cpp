// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Debug_Tool.h"
#define pr(text)


// Sets default values
ADebug_Tool::ADebug_Tool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADebug_Tool::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADebug_Tool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Str = "String Print to Console.............................................";
	vec = FVector(1.0f, 2.0f, 3.0f);
	
	//PrintS(Str);
	PrintV(vec);
	//printT(FString::Printf(TEXT("Test................"));
	pr(Str);
}

void ADebug_Tool::PrintS(FString String)
{
	UE_LOG(LogTemp, Warning, TEXT("String %s"), *String);
}

void ADebug_Tool::PrintV(FVector Vector)
{
	UE_LOG(LogTemp, Warning, TEXT("String %s"), *Vector.ToString());
}