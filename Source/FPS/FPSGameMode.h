// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"




USTRUCT(BlueprintType)
struct FPlayerData
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	FName PlayerName;
	UPROPERTY(BlueprintReadWrite)
	TArray<FVector> PlayerLocation;
	UPROPERTY(BlueprintReadWrite)
	TArray<float> PlayerScore;
	UPROPERTY(BlueprintReadWrite)
	FColor PlayerColor;
	UPROPERTY(BlueprintReadWrite)
	bool bIsPlayerDead;
	UPROPERTY(BlueprintReadWrite)
	TArray<FRotator> PlayerLockAtRotator;
	UPROPERTY(BlueprintReadWrite)
	TArray<double> Time;
};


UCLASS(minimalapi)
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AFPSGameMode();
	
	virtual void OnPostLogin(AController* NewPlayer) override;
	virtual void Logout(AController* Exiting) override;
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FColor> PlayerColors;
	
	UPROPERTY(BlueprintReadWrite)
	TArray<FName> PlayerDefaultNames;
	
	FTimerHandle Timer;
	
	bool GetPlayerData(FName PlayerName)const;
	
	TArray<FPlayerData> PlayerDatas;

	void UpdatePlayerLocation();
	UPROPERTY()
	TArray<APlayerController*> PlayerControllers;
private:


};



