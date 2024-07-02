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
	float GetTransitionTimerLength() const { return TransitionTimerLength; }

	UFUNCTION(BlueprintCallable)
	int GetCurrentGame() const { return CurrentGame; }

	class APunchSpawner* GetPunchSpawner() { return PunchSpawner; }

protected:
	virtual void BeginPlay() override;

	void RunMicroGame();

	UFUNCTION(BlueprintImplementableEvent)
	void RunMicroGameUI();

	void RunTransitionPhase();

	UFUNCTION(BlueprintImplementableEvent)
	void RunTransitionPhaseUI();

	void EndPhaseTransition();

	UFUNCTION(BlueprintImplementableEvent)
	void RunGame1UI();

	UFUNCTION(BlueprintImplementableEvent)
	void RunGame2UI();

	UFUNCTION(BlueprintImplementableEvent)
	void RunGame3UI();

	UPROPERTY(EditDefaultsOnly)
	float GameTimerLength;
	UPROPERTY(EditDefaultsOnly)
	float TransitionTimerLength;

	UPROPERTY(EditDefaultsOnly, Category="Game 2 Settings")
	float Game2CameraDistance = 900.f;

	UPROPERTY(EditDefaultsOnly, Category = "Game 3 Settings")
	float Game3CameraDistance = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Game 3 Settings")
	TSubclassOf<class APunchSpawner> PunchSpawnerClass;

private:

	class ACameraPawn* PlayerPawn;

	class AThePunchPlayerController* PlayerController;

	class APunchSpawner* PunchSpawner;

	void HandleStartup();

	void Game2Startup();

	void Game3Startup();

	void Game4Startup();

	TArray<float> GameScores;

	int CurrentGame;
	
};
