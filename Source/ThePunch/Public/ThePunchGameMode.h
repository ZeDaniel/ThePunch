// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ThePunchGameMode.generated.h"

/**
 * 
 */
UCLASS()
class THEPUNCH_API AThePunchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	float GetGameTimerLength() const { return GameTimerLength; }

	UFUNCTION(BlueprintCallable)
	float GetStartTransitionTimerLength() const { return StartTransitionTimerLength; }
	UFUNCTION(BlueprintCallable)
	float GetGameTransitionTimerLength() const { return GameTransitionTimerLength; }

	UFUNCTION(BlueprintCallable)
	int GetCurrentGame() const { return CurrentGame; }

	class APunchSpawner* GetPunchSpawner() { return PunchSpawner; }

	void CallTransitionPhaseManually();

	void QuitGame();

	UFUNCTION(BlueprintImplementableEvent)
	void RestartGame();


protected:
	virtual void BeginPlay() override;

	void RunMicroGame();

	UFUNCTION(BlueprintImplementableEvent)
	void RunMicroGameUI();

	void RunTransitionPhase();

	UFUNCTION(BlueprintImplementableEvent)
	void RunTransitionPhaseUI();
	UFUNCTION(BlueprintImplementableEvent)
	void RunStartTransitionPhaseUI();

	void TransitionIntoMicroGame();

	void MicroIntoTransitionPhase();

	UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
	float GameTimerLength;
	UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
	float StartTransitionTimerLength = 3.f;
	UPROPERTY(EditDefaultsOnly, Category = "Game Settings")
	float GameTransitionTimerLength = 1.f;

	UPROPERTY(EditDefaultsOnly, Category="Game 2 Settings")
	float Game2CameraDistance = 900.f;

	UPROPERTY(EditDefaultsOnly, Category = "Game 3 Settings")
	float Game3CameraDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Game 3 Settings")
	TSubclassOf<class APunchSpawner> PunchSpawnerClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game 4 Settings")
	TSubclassOf<class AScalableSprite> ArrowSpriteClass;

	UPROPERTY(EditDefaultsOnly, Category = "Game 4 Settings")
	float Game4CameraDistance = 1100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Game 4 Settings")
	float ArrowXOffset = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Game 4 Settings")
	float ArrowZOffset = 0.f;

private:

	class ACameraPawn* PlayerPawn;

	class ACameraPawn* BagPawn;

	class AThePunchPlayerController* PlayerController;

	class APunchSpawner* PunchSpawner;

	void HandleStartup();

	void Game2Startup();
	void Game3Startup();
	void Game4Startup();
	void Game5Startup();

	void EndGame();

	TArray<float> GameScores;

	int CurrentGame;
	
};
