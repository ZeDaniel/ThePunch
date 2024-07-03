// Fill out your copyright notice in the Description page of Project Settings.


#include "ThePunchGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "CameraPawn.h"
#include "ScalableSprite.h"
#include "ThePunchPlayerController.h"
#include "PunchSpawner.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

void AThePunchGameMode::QuitGame()
{
	UE_LOG(LogTemp, Display, TEXT("Quit action called"));
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, false);
}

void AThePunchGameMode::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle StartTimer;
	FTimerDelegate StartTimerDelegate = FTimerDelegate::CreateUObject(this, & AThePunchGameMode::HandleStartup);
	GetWorldTimerManager().SetTimer(StartTimer, StartTimerDelegate, 0.5f, false);

}

void AThePunchGameMode::RunMicroGame()
{
	if (CurrentGame == 1)
	{
		PlayerController->RunGame1UI();
		RunMicroGameUI();
		MicroIntoTransitionPhase();
	}
	else if (CurrentGame == 2)
	{
		PlayerController->RunGame2UI();
		RunMicroGameUI();
		MicroIntoTransitionPhase();
	}	
	else if (CurrentGame == 3)
	{
		PlayerController->RunGame3UI();
		PunchSpawner->ActivateSpawner();
		RunMicroGameUI();
		MicroIntoTransitionPhase();
	}
	else if (CurrentGame == 4)
	{
		PlayerController->RunGame4UI();

		FVector SpringArmLocation = PlayerPawn->GetSpringArmLocation();
		FVector ArrowLocation(SpringArmLocation.X + ArrowXOffset, SpringArmLocation.Y - Game4CameraDistance + 70, SpringArmLocation.Z + ArrowZOffset);
		FTransform ArrowTransform(FRotator::ZeroRotator, ArrowLocation);

		PlayerPawn->SpawnLaunchArrow(ArrowSpriteClass, ArrowTransform);

		RunMicroGameUI();
		MicroIntoTransitionPhase();
	}
	else if (CurrentGame == 5)
	{
		EndGame();
		BagPawn->Launch(PlayerPawn->GetConfirmedLaunchAngle(), PlayerPawn->GetLaunchPower());
	}

	PlayerController->SetPlayerEnabledState(true);
}

void AThePunchGameMode::RunTransitionPhase()
{
	if (CurrentGame == 1)
	{
		PlayerPawn->ConfirmCharge();
		PlayerController->SetPlayerEnabledState(false);
		GameScores.Add(PlayerPawn->GetConfirmedCharge());

		Game2Startup();
		TransitionIntoMicroGame();
	}
	else if (CurrentGame == 2)
	{
		PlayerController->SetPlayerEnabledState(false);
		PlayerPawn->ConfirmAccuracy();
		GameScores.Add(PlayerPawn->GetConfirmedAccuracy());

		Game3Startup();
		TransitionIntoMicroGame();
	}
	else if (CurrentGame == 3)
	{
		PlayerController->SetPlayerEnabledState(false);
		Game4Startup();
		TransitionIntoMicroGame();
	}
	else if (CurrentGame == 4)
	{
		PlayerController->SetPlayerEnabledState(false);
		EndGame();
		TransitionIntoMicroGame();
	}
}

void AThePunchGameMode::TransitionIntoMicroGame()
{
	RunTransitionPhaseUI();

	FTimerHandle StartTimer;
	FTimerDelegate StartTimerDelegate = FTimerDelegate::CreateUObject(this, &AThePunchGameMode::RunMicroGame);
	GetWorldTimerManager().SetTimer(StartTimer, StartTimerDelegate, GameTransitionTimerLength, false);
}

void AThePunchGameMode::MicroIntoTransitionPhase()
{
	FTimerHandle GameTimer;
	FTimerDelegate GameTimerDelegate = FTimerDelegate::CreateUObject(this, &AThePunchGameMode::RunTransitionPhase);
	GetWorldTimerManager().SetTimer(GameTimer, GameTimerDelegate, GameTimerLength, false);
}

void AThePunchGameMode::HandleStartup()
{
	PlayerPawn = Cast<ACameraPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	TArray<AActor*> ReturnedActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "BagPawn", ReturnedActors);

	BagPawn = Cast<ACameraPawn>(ReturnedActors[0]);

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

	RunStartTransitionPhaseUI();

	FTimerHandle StartTimer;
	FTimerDelegate StartTimerDelegate = FTimerDelegate::CreateUObject(this, &AThePunchGameMode::RunMicroGame);
	GetWorldTimerManager().SetTimer(StartTimer, StartTimerDelegate, StartTransitionTimerLength, false);
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
		PunchSpawner->SetSpawnRoot(BagPawn->GetSpringArmLocation());
		PunchSpawner->ScaleSpawnZone(FMath::Max(PlayerPawn->GetConfirmedAccuracy()-.25, 1));
	}
}

void AThePunchGameMode::Game4Startup()
{
	CurrentGame = 4;

	PlayerPawn->ChangeMappingContext(CurrentGame);
	PlayerController->SetCursorVisibility(false);
	PlayerPawn->MoveCameraToPunchBag(Game4CameraDistance);
}

void AThePunchGameMode::EndGame()
{
	PlayerController->UnPossess();
	if (BagPawn)
	{
		PlayerController->Possess(BagPawn);
	}

	CurrentGame = 5;

	PlayerPawn->ChangeMappingContext(CurrentGame);
	
	PlayerController->RunEndGameUI();
}
