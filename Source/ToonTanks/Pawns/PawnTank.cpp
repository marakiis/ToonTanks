// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnTank.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APawnTank::APawnTank() 
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
	Super::BeginPlay();

	PlayerControllerRef = Cast<APlayerController>(GetController());
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PlayerControllerRef) {
		FHitResult TraceHitResult;
		PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
		FVector HitLocation = TraceHitResult.ImpactPoint;
		RotateTurret(HitLocation);
		//RotateTurret(RotateTurretDirection);
	}

	if (bFire) 
	{
		Fire();
	}

	Rotate();
	Move();
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
	PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::CalculateFire);
	PlayerInputComponent->BindAxis("RotateTurret", this, &APawnTank::CalculateRotateTurretInput);
}

void APawnTank::HandleDestruction() {
    Super::HandleDestruction();
    // Deactivate Actor
	bIsAlive = false;

	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);
}

bool APawnTank::IsAlive() 
{
	return bIsAlive;
}

void APawnTank::Fire()
{
	Super::Fire();
	bFire = false;
	LastFire = GetWorld()->GetTimeSeconds();
}

void APawnTank::CalculateMoveInput(float Value) 
{
	MoveDirection = FVector(Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::CalculateRotateInput(float Value) 
{
	float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
	FRotator Rotation = FRotator(0, RotateAmount, 0);
	RotationDirection = FQuat(Rotation);
}

void APawnTank::CalculateRotateTurretInput(float Value)
{
	RotateTurretDirection += FVector(0, Value, 0);
}

void APawnTank::CalculateFire()
{
	if (GetWorld()->GetTimeSeconds() - LastFire < FireRate)
	{
		return;
	}
	bFire = true;
}

void APawnTank::Move() 
{
	AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate() 
{
	AddActorLocalRotation(RotationDirection, true);
}
