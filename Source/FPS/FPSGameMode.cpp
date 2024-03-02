// Copyright Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "GameFramework/PlayerState.h"
#include "UObject/ConstructorHelpers.h"
#include "UObject/UnrealTypePrivate.h"

AFPSGameMode::AFPSGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
}

void AFPSGameMode::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	
	if(APlayerController* PC = CastChecked<APlayerController>(NewPlayer))
	{
		PlayerControllers.Add(PC);
	
		auto& NewPlayerData = PlayerDatas.AddDefaulted_GetRef();
		if(PlayerDefaultNames.Num()!= 0 && PlayerColors.Num()!=0)
		{
			const FName DefaultPlayer = PlayerDefaultNames[FMath::RandRange(0,PlayerDefaultNames.Num()-1)];
			NewPlayerData.PlayerName = DefaultPlayer;

			
			PC->SetName(DefaultPlayer.ToString());
			PlayerDefaultNames.Remove(DefaultPlayer);

			const FColor Color = PlayerColors[FMath::RandRange(0,PlayerDefaultNames.Num()-1)];
			NewPlayerData.PlayerColor = Color;
			PlayerColors.Remove(Color);
		}
	}
}

void AFPSGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	PlayerControllers.Remove(CastChecked<APlayerController>(Exiting));
	
}


void AFPSGameMode::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(Timer,this,
		&AFPSGameMode::UpdatePlayerLocation,1.f,true);
}


bool AFPSGameMode::GetPlayerData(FName PlayerName)const
{
	for (auto i : PlayerDatas)
	{
		if(i.PlayerName == PlayerName)
		{
			for (auto IPlayerController : PlayerControllers)
			{
				APlayerState* PlayerState = IPlayerController->GetPlayerState<APlayerState>();
				
				if(PlayerState->GetPlayerName() == PlayerName)
				{
					AFPSCharacter* Character = Cast<AFPSCharacter>(IPlayerController->GetPawn());
					
					Character->ValueChange(PlayerDatas);
					//Character->PlayerData = PlayerDatas;
					return true;
				}
			}
		}
	}
	return false;
}

void AFPSGameMode::UpdatePlayerLocation()
{
	if(PlayerControllers.Num() == 0 or PlayerDatas.Num() == 0)
	{
		return;
	}
	
	for (const auto i : PlayerControllers)
	{
		if(APawn* Pawn = i->GetPawn())
		{
			for (auto PlayerData = PlayerDatas.CreateIterator();PlayerData;++PlayerData)
			{
				PlayerData->PlayerLocation.Add(Pawn->GetActorLocation());
				PlayerData->Time.Add(GetWorld()->GetTime().GetWorldTimeSeconds());
				//反射添加分数。
				const FFloatProperty* Property2 = Cast<FFloatProperty>(Pawn->GetClass()->FindPropertyByName(FName("Score")));
				if(Property2)
				{
					float Score = Property2->GetPropertyValue(Property2->ContainerPtrToValuePtr<void>(Pawn));
					PlayerData->PlayerScore.Add(Score);
				}
				
				FRotator Rotator = Pawn->GetActorRotation();
				PlayerData->PlayerLockAtRotator.Add(Rotator);
			}
		}						
	}
}

