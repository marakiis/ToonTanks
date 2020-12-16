// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTurret.h"
#include "PawnTank.h"
#include "Kismet/GameplayStatics.h"

APawnTurret::APawnTurret() 
{

}

// Called when the game starts or when spawned
void APawnTurret::BeginPlay()
{
	Super::BeginPlay();

    GetWorld()->GetTimerManager().SetTimer(FireRateTimerHandle, this, &APawnTurret::CheckFireCondition, FireRate, true);

    PlayerPawn = Cast<APawnTank>(UGameplayStatics::GetPlayerPawn(this, 0));
}

// Called every frame
void APawnTurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!PlayerPawn) {
        return;
    }

    RotateTurret(PlayerPawn->GetActorLocation());

    if (bFire) {
        Fire();
    } 
}

void APawnTurret::HandleDestruction() {
    Super::HandleDestruction();
    Destroy();
}

void APawnTurret::Fire() 
{
    Super::Fire();
    bFire = false;
}

void APawnTurret::CheckFireCondition() 
{
    if (!PlayerPawn || !PlayerPawn->IsAlive())
    {
        return;
    }
    // If Player Is in range FIRE
    if(GetDistanceToPlayer() > FireRange)
    {
        return;
    }
    
    bFire = true;
}

float APawnTurret::GetDistanceToPlayer() 
{
    if (!PlayerPawn) {
        return 0.f;
    }

    return FVector::Dist(PlayerPawn->GetActorLocation(), GetActorLocation());
}

