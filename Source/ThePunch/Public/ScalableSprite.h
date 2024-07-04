// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ScalableSprite.generated.h"

UCLASS()
class THEPUNCH_API AScalableSprite : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AScalableSprite();

	float GetSpriteScale() const { return SpriteScale; }

	void SetIsMovingSprite(bool IsMoving) { IsMovingSprite = IsMoving; }

	float GetRotateSpeed() { return RotateSpeed; }
	void SetRotateSpeed(float NewSpeed) { RotateSpeed = NewSpeed; }

	void UpdateSpriteScale(float NewSpriteScale);

	void HandleDestruction();

protected:
	// Called when the game starts or when spawned
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere)
	class UPaperSpriteComponent* SpriteComp;
	
	UPROPERTY(EditAnywhere, Category = "Move Settings")
	float RotateMax = 45.f;
	UPROPERTY(EditAnywhere, Category = "Move Settings")
	float RotateMin = -45.f;
	UPROPERTY(EditAnywhere, Category = "Move Settings")
	float RotateSpeed = 5.f;

	UPROPERTY(EditAnywhere)
	class USoundWave* SpriteSpawnSound;
	UPROPERTY(EditAnywhere)
	class USoundWave* SpriteDeathSound;

	virtual void BeginPlay() override;

	float SpriteScale = 1.f;

private:

	bool IsMovingSprite = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
