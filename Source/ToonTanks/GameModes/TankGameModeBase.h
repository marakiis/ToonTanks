// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TankGameModeBase.generated.h"


class APawnTank;
class APawnTurret;
class APlayerControllerBase;
UCLASS()
class TOONTANKS_API ATankGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void ActorDied(AActor* DeadActor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Loop")
	int32 StartDelay = 3;
	APlayerControllerBase* PlayerControllerRef = nullptr;

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintImplementableEvent)
	void GameStart();
	UFUNCTION(BlueprintImplementableEvent)
	void GameOver(bool bPlayerWon);

private:
	// VARIABLES
	APawnTank  *PlayerTank;
	int32 TargetTurrets = 0;
	
	// FUNCTIONS
	void HandleGameStart();
	void HandleGameOver(bool bPlayerWon);
	int32 GetTargetTurretsCount();
};
