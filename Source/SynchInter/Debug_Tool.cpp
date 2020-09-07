// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Debug_Tool.h"
#include "Engine.h"

UDebug_Tool::UDebug_Tool(){}

void UDebug_Tool::PrintS(FString String)
{
	UE_LOG(LogTemp, Warning, TEXT("String %s"), *String);
}

void UDebug_Tool::PrintV(FVector Vector)
{
	UE_LOG(LogTemp, Warning, TEXT("String %s"), *Vector.ToString());
}

void UDebug_Tool::PrintString(FString string)
{
	UE_LOG(LogTemp, Warning, TEXT("String: %s"), *string);
}

void UDebug_Tool::PrintInt(int num)
{
	FString string = FString::FromInt(num);
	UE_LOG(LogTemp, Warning, TEXT("Int To String: %s"), *string);
}

void UDebug_Tool::PrintFloat(float flt) 
{
	FString string = FString::SanitizeFloat(flt);
	UE_LOG(LogTemp, Warning, TEXT("Float To String: %s"), *string);
}

void UDebug_Tool::PrintBool(bool bol) 
{
	FString name = FString(bol ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Warning, TEXT("String: %s"), *name);
}

void UDebug_Tool::PrintVec(FVector vector) {
	FString string = vector.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Actor Transform To String: %s"), *string);
}

void UDebug_Tool::PrintInt32WitStr(FString String2, int32 num)
{
	FString String1 = String2;
	FString string = FString::FromInt(num);
	UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *String1,*string);
}

void UDebug_Tool::PrintActTrans(FTransform trans) 
{
	FString string = trans.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Actor Transform To String: %s"), *string);
}

void UDebug_Tool::PrintStringToInt(FString string) 
{
	FString StrToInt = string;
	int32 integer = FCString::Atoi(*StrToInt);
	UE_LOG(LogTemp, Warning, TEXT("%d"), integer);
}


void UDebug_Tool::PrintFloatWitStr(FString String2, float flt)
{
	FString String1 = String2;
	FString Flot = FString::SanitizeFloat(flt);
	UE_LOG(LogTemp, Warning, TEXT("%s: %s"),*String1,*Flot);

}

void UDebug_Tool::PrintVectorWitStr(FString String, FVector Vector) 
{
	FString _String = String;
	FString _Vector = Vector.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *_String, *_Vector);
}

void UDebug_Tool::PrintActTraWitStr(FString String, FTransform Transform)
{
	FString _String = String;
	FString _Transform = Transform.ToString();
	UE_LOG(LogTemp, Warning, TEXT("%s: %s"), *_String, *_Transform);
}

void UDebug_Tool::AddToOnScreen(FString string)
{
	GEngine->AddOnScreenDebugMessage(1, 0.0f, FColor::Yellow, string, true, FVector2D(1.3, 1.3));
}

/********************************************************************
// Defines for shortcuts to be added to classes your working on, Copy Paste

// Signature for Declartation file

UDebug_Tool* pr;

//Define short cuts for below the includes

#define prVec(text)				pr->PrintVec(text)
#define prStr(text)				pr->PrintString(text)
#define prInt(text)				pr->PrintInt(text)
#define prFloat(text)			pr->PrintFloat(text)
#define prBool(text)			pr->PrintBool(text)
#define prActTrans(text)		pr->PrintActTrans(text)
#define prStringToInt(text)		pr->PrintStringToInt(text)

//Print variable with string, description before variable

#define prFltWitStr(text,flt)	pr->PrintFloatWitStr(text,flt)
#define prVecWitStr(text,flt)	pr->PrintVectorWitStr(text,flt)

*********************************************************************/








/*
// Called when the game starts or when spawned
void ADebug_Tool::BeginPlay()
{
	Super::BeginPlay();
}
*/
/*
// Called every frame
void ADebug_Tool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Str = "String Print to Console.............................................";
	vec = FVector(1.0f, 2.0f, 3.0f);
	
	//PrintS(Str);
	PrintV(vec);
	//printT(FString::Printf(TEXT("Test................"));
	//pr(Str);
}
*/