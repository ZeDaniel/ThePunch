// Fill out your copyright notice in the Description page of Project Settings.


#include "PunchSpawner.h"
#include "ScalableSprite.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APunchSpawner::APunchSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void APunchSpawner::ActivateSpawner()
{
	IsActive = true;
}

void APunchSpawner::ClearSpawner()
{
	for (AScalableSprite* Sprite : SpawnedSprites)
	{
		UE_LOG(LogTemp, Display, TEXT("Sprite destroyed"));
		Sprite->HandleDestruction();
	}
}

void APunchSpawner::ScaleSpawnZone(float TightenScalar)
{
	float XSpawnRange = MaxXSpawn - MinXSpawn;
	float XMidpoint = MinXSpawn + (XSpawnRange / 2);
	float ZSpawnRange = MaxZSpawn - MinZSpawn;
	float ZMidpoint = MinZSpawn + (ZSpawnRange / 2);

	XSpawnRange = XSpawnRange / TightenScalar;
	ZSpawnRange = ZSpawnRange / TightenScalar;

	MinXSpawn = XMidpoint - (XSpawnRange / 2);
	MaxXSpawn = XMidpoint + (XSpawnRange / 2);
	MinZSpawn = ZMidpoint - (ZSpawnRange / 2);
	MaxZSpawn = ZMidpoint + (ZSpawnRange / 2);	
}

/*
If punch was successful, function removes punch and returns true
If punch was unsuccessful, funciton returns false
*/
bool APunchSpawner::HandlePunch(FHitResult* ThrownPunchHitResult)
{
	if (IsActive)
	{
		if (ThrownPunchHitResult && ThrownPunchHitResult->bBlockingHit && ThrownPunchHitResult->GetActor())
		{	
			if (ThrownPunchHitResult->GetActor()->ActorHasTag(FName("SpawnedPunch")))
			{
				AScalableSprite* HitSprite = Cast<AScalableSprite>(ThrownPunchHitResult->GetActor());
				int index = SpawnedSprites.IndexOfByKey(HitSprite);
				SpawnedSprites.RemoveAtSwap(index);
				HitSprite->HandleDestruction();
				return true;
			}
		}
	}
	return false;
}

// Called when the game starts or when spawned
void APunchSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

void APunchSpawner::SpawnPunch()
{
	FVector SpawnLocation(UKismetMathLibrary::RandomFloatInRange(SpawnRoot.X + MinXSpawn, SpawnRoot.X + MaxXSpawn), YSpawn, UKismetMathLibrary::RandomFloatInRange(SpawnRoot.Z + MinZSpawn, SpawnRoot.Z + MaxZSpawn));
	auto SpawnedPunch = GetWorld()->SpawnActor<AScalableSprite>(SpawnSpriteClass, SpawnLocation, FRotator::ZeroRotator);
	SpawnedPunch->Tags.Add(FName("SpawnedPunch"));

	SpawnedSprites.Add(SpawnedPunch);
}

void APunchSpawner::HandleDeath()
{
	ClearSpawner();
	IsActive = false;

	Destroy();
}

// Called every frame
void APunchSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsActive)
	{
		ActiveLifeTime += DeltaTime;
		TimeSinceLastSpawn += DeltaTime;

		if (ActiveLifeTime > SpawnerLifetime)
		{
			HandleDeath();
		}
		else if (TimeSinceLastSpawn > SpawnInterval)
		{
			TimeSinceLastSpawn = 0;
			SpawnPunch();
		}
	}	
}

