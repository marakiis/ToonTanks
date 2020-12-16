// Fill out your copyright notice in the Description page of Project Settings.

#include "TankGameModeBase.h"
#include "ToonTanks/Pawns/PawnTank.h"
#include "ToonTanks/Pawns/PawnTurret.h"
#include "ToonTanks/PlayerControllers/PlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"

void ATankGameModeBase::BeginPlay()
{
    Super::BeginPlay();

    HandleGameStart();
}

void ATankGameModeBase::ActorDied(AActor *DeadActor)
{
    if (DeadActor == PlayerTank)
    {
        PlayerTank->HandleDestruction();
        if(PlayerControllerRef) 
        {
            PlayerControllerRef->SetPlayerEnabledState(false);
        }
        HandleGameOver(false);
    }
    else if (APawnTurret *DestroyedTurret = Cast<APawnTurret>(DeadActor))
    {
        DestroyedTurret->HandleDestruction();
        if (--TargetTurrets <= 0)
        {
            HandleGameOver(true);
        }
        UE_LOG(LogTemp, Warning, TEXT("%d turrets remaining!"), TargetTurrets);
    }
}

void ATankGameModeBase::HandleGameStart()
{
    TargetTurrets = GetTargetTurretsCount();
    PlayerTank = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
    PlayerControllerRef= Cast<APlayerControllerBase>(UGameplayStatics::GetPlayerController(this, 0));
    GameStart();

    if (PlayerControllerRef)
    {
        PlayerControllerRef->SetPlayerEnabledState(false);
        FTimerHandle PlayerEnableHandle;
        FTimerDelegate PlayerEnableDelegate = FTimerDelegate::CreateUObject(PlayerControllerRef,
            &APlayerControllerBase::SetPlayerEnabledState, true);
        GetWorld()->GetTimerManager().SetTimer(PlayerEnableHandle, PlayerEnableDelegate, StartDelay, false);
    }
}

void ATankGameModeBase::HandleGameOver(bool bPlayerWon)
{
    GameOver(bPlayerWon);
}

int32 ATankGameModeBase::GetTargetTurretsCount()
{
    // Get number of turret
    TArray<AActor *> FoundActor;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APawnTurret::StaticClass(), FoundActor);
    return FoundActor.Num();
}