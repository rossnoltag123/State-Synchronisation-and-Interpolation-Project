// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Debug_Tool.generated.h"

//#define printT(text)			if(GEngine) GEngine->AddOnScreenDebugMessage(-1,1.5f, FColor::White, TEXT(text), false)
//#define pr(text)		UE_LOG(LogTemp, Warning, TEXT("text"));

#define printT(text)			if(GEngine) GEngine->AddOnScreenDebugMessage(-1,1.5f, FColor::White, TEXT(text), false)
#define pr(text)		UE_LOG(LogTemp, Warning, TEXT("text"))

UCLASS()
class SYNCHINTER_API ADebug_Tool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADebug_Tool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	void PrintS(FString String);

	UFUNCTION()
	void PrintV(FVector Vector);

	FVector vec;
	FString Str;
};
