// Fill out your copyright notice in the Description page of Project Settings.


#include "ThePunchGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "CameraPawn.h"
#include "ScalableSprite.h"
#include "ThePunchPlayerController.h"
#include "PunchSpawner.h"

void AThePunchGameMode::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle StartTimer;
	FTimerDelegate StartTimerDelegate = FTimerDelegate::CreateUObject(this, & AThePunchGameMode::HandleStartup);
	GetWorldTimerManager().SetTimer(StartTimer, StartTimerDelegate, 0.5f, false);

}

void AThePunchGameMode::RunMicroGame()
{
	RunMicroGameUI();

	if (CurrentGame == 1)
		RunGame1UI();
	else if (CurrentGame == 2)
		RunGame2UI();
	else if (CurrentGame == 3)
	{
		RunGame3UI();
		PunchSpawner->ActivateSpawner();
	}

	PlayerController->SetPlayerEnabledState(true);

	FTimerHandle GameTimer;
	FTimerDelegate GameTimerDelegate = FTimerDelegate::CreateUObject(this, &AThePunchGameMode::RunTransitionPhase);
	GetWorldTimerManager().SetTimer(GameTimer, GameTimerDelegate, GameTimerLength, false);

}

void AThePunchGameMode::RunTransitionPhase()
{
	if (CurrentGame == 1)
	{
		PlayerPawn->ConfirmCharge();
		PlayerController->SetPlayerEnabledState(false);
		GameScores.Add(PlayerPawn->GetConfirmedCharge());

		Game2Startup();
	}
	else if (CurrentGame == 2)
	{
		PlayerController->SetPlayerEnabledState(false);
		PlayerPawn->ConfirmAccuracy();
		GameScores.Add(PlayerPawn->GetConfirmedAccuracy());

		Game3Startup();
	}
	else if (CurrentGame == 3)
	{
		Game4Startup();
		//TODO: REMOVE ASAP
		return;
	}

	RunTransitionPhaseUI();

	FTimerHandle StartTimer;
	FTimerDelegate StartTimerDelegate = FTimerDelegate::CreateUObject(this, &AThePunchGameMode::RunMicroGame);
	GetWorldTimerManager().SetTimer(StartTimer, StartTimerDelegate, TransitionTimerLength, false);
}

void AThePunchGameMode::HandleStartup()
{
	PlayerPawn = Cast<ACameraPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	PlayerController = Cast<AThePunchPlayerController>(PlayerPawn->GetController());

	PlayerController->SetPlayerEnabledState(false);
	PlayerPawn->SetCrosshairVisibility(false);

	TArray<AActor*> OutActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AScalableSprite::StaticClass(), OutActors);

	for (AActor* Actor : OutActors)
	{
		AScalableSprite* Sprite = Cast<AScalableSprite>(Actor);
		if (Sprite)
		{
			PlayerPawn->AddFist(Sprite);
		}
	}

	CurrentGame = 1;

	RunTransitionPhaseUI();

	FTimerHandle StartTimer;
	FTimerDelegate StartTimerDelegate = FTimerDelegate::CreateUObject(this, &AThePunchGameMode::RunMicroGame);
	GetWorldTimerManager().SetTimer(StartTimer, StartTimerDelegate, TransitionTimerLength, false);
}

void AThePunchGameMode::Game2Startup()
{
	CurrentGame = 2;

	PlayerPawn->ChangeMappingContext(CurrentGame);
	PlayerPawn->SetCrosshairVisibility(true);
	PlayerPawn->MoveCameraToPunchBag(Game2CameraDistance);
	
}

void AThePunchGameMode::Game3Startup()
{
	CurrentGame = 3;

	PlayerPawn->ChangeMappingContext(CurrentGame);
	PlayerPawn->SetCrosshairVisibility(false);
	PlayerPawn->MoveCameraToPunchBag(Game3CameraDistance);
	PlayerController->SetCursorVisibility(true);

	PunchSpawner = GetWorld()->SpawnActor<APunchSpawner>(PunchSpawnerClass);
	if (PunchSpawner)
	{
		PunchSpawner->SetOwner(this);
	}
}

void AThePunchGameMode::Game4Startup()
{
	CurrentGame = 4;
}
