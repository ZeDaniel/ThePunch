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
	float GetCurrentAccuracy() const;
	UFUNCTION(BlueprintCallable)
	float GetConfirmedAccuracy() const { return ConfirmedAccuracy; }
	UFUNCTION(BlueprintCallable)
	int GetSuccessfulPunches() const { return SuccessfulPunchCount; }
	UFUNCTION(BlueprintCallable)
	FRotator GetLaunchArrowRotation();
	UFUNCTION(BlueprintCallable)
	float GetDistanceFlown() const { return (GetActorLocation().X - InitialXOffset)/100; }

	void SetFistOrigin(FVector Origin);

	FVector GetSpringArmLocation();
	
	float GetConfirmedAccuracy() { return ConfirmedAccuracy; }

	float GetConfirmedLaunchAngle() { return ConfirmedLaunchAngle; }

	float GetLaunchPower();

	void AddFist(class AScalableSprite* Fist) { Fists.Add(Fist); }

	void ConfirmCharge();

	void ConfirmAccuracy();

	void ChangeMappingContext(int Game);

	void SetCrosshairVisibility(bool IsVisible);

	void MoveCameraToPunchBag(float CameraDistanceFromBag);

	void SpawnLaunchArrow(UClass* ArrowClass, FTransform ArrowTransform);

	void Launch(float LaunchAngle, float LaunchPower);

	void PlayCountdownSound();
	void PlayCountdownSound(float SoundDuration);
	void StopCountdownSound();

	void PlayMicroStartSound();
	void PlayMicroStartSound(float delay);

	void PlayCrosshairSound(float SoundDuration);

	void PlayWindSound();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	class UCapsuleComponent* CapsuleComp;

	UPROPERTY(EditDefaultsOnly)
	class UPaperSpriteComponent* SpriteComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly)
	class UCameraComponent* CameraComp;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* AudioComp;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* CountdownAudioComp;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* MicroStartAudioComp;

	UPROPERTY(EditDefaultsOnly)
	class UAudioComponent* BagDragAudioComp;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* RestartGameAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* QuitGameAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* BaseMappingContext;

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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* LaunchAngleMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ConfirmLaunchAction;

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
	float LaunchScalar = 100000.f;
	UPROPERTY(EditAnywhere)
	float AngularLaunchScalar = 100000.f;

	UPROPERTY(EditAnywhere)
	float WindSoundStrength = 1.f;

	UPROPERTY(EditAnywhere)
	TArray<class AScalableSprite*> Fists;

	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundWave* CountdownSound;
	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundWave* CrosshairSound;
	UPROPERTY(EditAnywhere, Category = "Sounds")
	TArray<class USoundWave*> PunchSounds;
	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundWave* ArrowSound;
	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundWave* LaunchSound;
	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundWave* WindSound;
	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundWave* ConfettiSound;
	UPROPERTY(EditAnywhere, Category = "Sounds")
	class USoundWave* HardCollisionSound;

	UPROPERTY(EditAnywhere)
	FVector FistOffset;

private:

	APlayerController* ThePunchPlayerController;

	ACameraPawn* BagPawn;

	class AThePunchGameMode* ThePunchGameMode;

	class AScalableSprite* LaunchArrow;

	float Charge = 0.f;

	float ConfirmedCharge = 0.1f;

	float ConfirmedAccuracy = -1.f;

	float ConfirmedLaunchAngle = 0.f;

	float InitialXOffset = 1760.f;

	void UpdateCharge(float DeltaTime);

	void UpdateCrosshair(float DeltaTime);

	void MoveCrosshair(const FInputActionValue& Value);

	float CrosshairTimer = -1.f;

	float CrosshairXDirection;
	float CrosshairZDirection;

	int SuccessfulPunchCount = 1;

	void ThrowPunch(const FInputActionValue& Value);

	void BoostChargeBar(const FInputActionValue& Value);

	void ConfirmLaunchAngle(const FInputActionValue& Value);

	void PlayPunchAnimation(FVector StartLocation, FVector TargetLocation, float DeltaTime, float InterpSpeed);

	void RestartGame();

	void QuitGame();

	float ChargeSoundsIndex = 0;

	void PlayChargeSound();

	void PlayPunchSound();

	void StartArrowSound();
	void StopArrowSound();

	void PlayLaunchSound(float LaunchPower);

	bool IsWindy = false;

	void UpdateWindSound(float DeltaTime);
	float WindSoundDivisor = 10000 / WindSoundStrength;

	void PlayConfettiSound();
	float ConfettiTimer = 1.f;

	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	float LastZVelocity = -5000.f;
	float LastXVelocity = -5000.f;
	float DragTimer = 0.01f;

	FVector FistOrigin;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
