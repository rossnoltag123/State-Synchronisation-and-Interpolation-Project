// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Debug_Tool.generated.h"

#define pr(text)	testHeader->PrintVec(text)


UCLASS()
class SYNCHINTER_API UDebug_Tool : public UObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	UDebug_Tool();

public:
	UFUNCTION()
	void PrintS(FString String);

	UFUNCTION()
	void PrintV(FVector Vector);

	FVector vec;
	FString Str;

public:
	void PrintString(FString string);
	void PrintInt(int num);
	void PrintFloat(float flt);
	void PrintBool(bool bol);
	void PrintVec(FVector Vector);
	void PrintActTrans(FTransform trans);
	void PrintStringToInt(FString string);
	void AddToOnScreen(FString string);
	FString VarToString(FString string);
	void PrintInt32WitStr(FString String2, int num);
	void PrintFloatWitStr(FString String, float flt);

	void PrintVectorWitStr(FString String, FVector Vector);

	void PrintActTraWitStr(FString String, FTransform PrintActTraWitStr);
};








//#define printT(text)			if(GEngine) GEngine->AddOnScreenDebugMessage(-1,1.5f, FColor::White, TEXT(text), false)
//#define pr(text)		UE_LOG(LogTemp, Warning, TEXT("text"));

//#define printT(text)			if(GEngine) GEngine->AddOnScreenDebugMessage(-1,1.5f, FColor::White, TEXT(text), false)
//#define pr(text)		UE_LOG(LogTemp, Warning, TEXT("text"))

/*
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
*/