// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PunchSpawner.generated.h"

UCLASS()
class THEPUNCH_API APunchSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APunchSpawner();

	void ActivateSpawner();

	void ClearSpawner();

	void SetSpawnRoot(FVector NewRoot) { SpawnRoot = NewRoot; }

	void ScaleSpawnZone(float TightenScalar);

	bool HandlePunch(FHitResult* ThrownPunchHitResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	float SpawnerLifetime = 3.f;
	UPROPERTY(EditAnywhere)
	float SpawnInterval = 0.1f;
	UPROPERTY(EditAnywhere)
	float SpawnLife = .5f;

	UPROPERTY(EditAnywhere)
	float YSpawn = 120.f;
	UPROPERTY(EditAnywhere)
	float MinXSpawn = 2500.f;
	UPROPERTY(EditAnywhere)
	float MaxXSpawn = 3500.f;
	UPROPERTY(EditAnywhere)
	float MinZSpawn = 85.f;
	UPROPERTY(EditAnywhere)
	float MaxZSpawn = 550.f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AScalableSprite> SpawnSpriteClass;

	TArray<class AScalableSprite*> SpawnedSprites;

private:

	bool IsActive = false;

	float ActiveLifeTime = 0.f;

	float TimeSinceLastSpawn = SpawnInterval;

	void SpawnPunch();

	void HandleDeath();

	float DeathTimerLength = 0.25f;

	FVector SpawnRoot = FVector::ZeroVector;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
