// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "CameraPawn.generated.h"

UCLASS()
class THEPUNCH_API ACameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACameraPawn();

	UFUNCTION(BlueprintCallable)
	float GetCharge() const { return Charge; }
	UFUNCTION(BlueprintCallable)
	float GetMaxCharge() const { return MaxCharge; }
	UFUNCTION(BlueprintCallable)
	float GetConfirmedCharge() { return ConfirmedCharge; }
	UFUNCTION(BlueprintCallable)
	float GetCurrentAccuracy();

	FVector GetSpringArmLocation();
	
	float GetConfirmedAccuracy() { return ConfirmedAccuracy; }

	void AddFist(class AScalableSprite* Fist) { Fists.Add(Fist); }

	void ConfirmCharge();

	void ConfirmAccuracy();

	void ChangeMappingContext(int Game);

	void SetCrosshairVisibility(bool IsVisible);

	void MoveCameraToPunchBag(float CameraDistanceFromBag);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditDefaultsOnly)
	class UPaperSpriteComponent* SpriteComp;

	UPROPERTY(EditDefaultsOnly)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* BoostBarMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* BoostChargeBarAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* AimCrosshairMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveCrosshairAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* ThrowPunchMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ThrowPunchAction;

	UPROPERTY(EditAnywhere)
	float MaxCharge = 5.f;

	UPROPERTY(EditAnywhere)
	float ChargeScalar = 5.f;

	UPROPERTY(EditAnywhere)
	float MaxAccuracyScore = 5.f;

	UPROPERTY(EditAnywhere)
	float CrosshairMovementScalar = 1.f;

	UPROPERTY(EditAnywhere)
	float CrosshairAimScalar = 1.5f;

	UPROPERTY(EditAnywhere)
	float CrosshairTimerInterval = 1.f;

	UPROPERTY(EditAnywhere)
	float CrosshairTimerDelay = 0.05f;

	UPROPERTY(EditAnywhere)
	float AccuracyScoreDivider = 50.f;

	UPROPERTY(EditAnywhere)
	TArray<class AScalableSprite*> Fists;

private:

	APlayerController* ThePunchPlayerController;

	ACameraPawn* BagPawn;

	class AThePunchGameMode* ThePunchGameMode;

	float Charge = 0.f;

	float ConfirmedCharge = 0.1f;

	float ConfirmedAccuracy = -1.f;

	void UpdateCharge(float DeltaTime);

	void UpdateCrosshair(float DeltaTime);

	void MoveCrosshair(const FInputActionValue& Value);

	float CrosshairTimer = 0.5f;

	float CrosshairXDirection;
	float CrosshairZDirection;

	int SuccessfulPunchCount = 0;

	void ThrowPunch(const FInputActionValue& Value);

	void BoostChargeBar(const FInputActionValue& Value);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
