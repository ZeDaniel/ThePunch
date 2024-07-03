// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraPawn.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "gameframework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "ScalableSprite.h"
#include "PaperSpriteComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ThePunchGameMode.h"
#include "PunchSpawner.h"

// Sets default values
ACameraPawn::ACameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = CapsuleComp;

	SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite Component"));
	SpriteComp->SetupAttachment(CapsuleComp);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("Springarm Component"));
	SpringArmComp->SetupAttachment(CapsuleComp);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp);

}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
	Super::BeginPlay();

	ThePunchPlayerController = Cast<APlayerController>(GetController());

	if (ThePunchPlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(ThePunchPlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BaseMappingContext, 0);
			Subsystem->AddMappingContext(BoostBarMappingContext, 1);
		}
	}

	TArray<AActor*> ReturnedActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), "BagPawn", ReturnedActors);

	BagPawn = Cast<ACameraPawn>(ReturnedActors[0]);

	ThePunchGameMode = Cast<AThePunchGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void ACameraPawn::UpdateCharge(float DeltaTime)
{
	if (Charge > 0.1)
	{
		Charge -= DeltaTime * ChargeScalar;
	}
	else if (Charge > MaxCharge)
	{
		Charge = MaxCharge;
	}
}

void ACameraPawn::UpdateCrosshair(float DeltaTime)
{
	if (CrosshairTimer < 0)
	{
		CrosshairXDirection = UKismetMathLibrary::RandomFloatInRange(-1, 1);
		CrosshairZDirection = UKismetMathLibrary::RandomFloatInRange(-1, 1);
		CrosshairTimer = CrosshairTimerInterval;
	}
	else
	{
		CrosshairTimer -= DeltaTime;
	}

	FVector DeltaLocation(DeltaTime * CrosshairMovementScalar * ConfirmedCharge * CrosshairXDirection, 0, DeltaTime * CrosshairMovementScalar * ConfirmedCharge * CrosshairZDirection);
	SpriteComp->AddLocalOffset(DeltaLocation);
}

void ACameraPawn::MoveCrosshair(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	FVector DeltaLocation(LookAxisValue.X, 0, LookAxisValue.Y);
	SpriteComp->AddLocalOffset(DeltaLocation*CrosshairAimScalar);
}

void ACameraPawn::BoostChargeBar(const FInputActionValue& Value)
{
	Charge += 0.5f;
}

void ACameraPawn::ConfirmLaunchAngle(const FInputActionValue& Value)
{
	LaunchArrow->SetIsMovingSprite(false);
	ConfirmedLaunchAngle = GetLaunchArrowRotation().Pitch;

	LaunchArrow->HandleDestruction();
}

void ACameraPawn::RestartGame()
{
	ThePunchGameMode->RestartGame();
}

void ACameraPawn::QuitGame()
{
	ThePunchGameMode->QuitGame();
}

float ACameraPawn::GetCurrentAccuracy() const
{
	FVector CrosshairLocation = SpriteComp->GetComponentLocation();
	FVector BagLocation = BagPawn->GetSpringArmLocation();
	CrosshairLocation.Y = BagLocation.Y;
	
	float DistanceFromTarget = FVector::Distance(CrosshairLocation, BagLocation);
	//return DistanceFromTarget;
	return FMath::Max(MaxAccuracyScore - DistanceFromTarget / AccuracyScoreDivider, 0.1);
}

FRotator ACameraPawn::GetLaunchArrowRotation()
{
	return LaunchArrow->GetActorRotation();
}

FVector ACameraPawn::GetSpringArmLocation()
{
	return SpringArmComp->GetComponentLocation();
}

float ACameraPawn::GetLaunchPower()
{
	return ConfirmedCharge * ConfirmedAccuracy * SuccessfulPunchCount;
}

void ACameraPawn::ConfirmCharge()
{
	ConfirmedCharge = FMath::Max(FMath::Min(Charge, MaxCharge), .1);
}

void ACameraPawn::ConfirmAccuracy()
{
	ConfirmedAccuracy = GetCurrentAccuracy();
}

void ACameraPawn::ChangeMappingContext(int Game)
{
	if (Game == 2)
	{
		if (ThePunchPlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(ThePunchPlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(BoostBarMappingContext);
				Subsystem->AddMappingContext(AimCrosshairMappingContext, 1);
			}
		}
	}
	else if (Game == 3)
	{
		if (ThePunchPlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(ThePunchPlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(AimCrosshairMappingContext);
				Subsystem->AddMappingContext(ThrowPunchMappingContext, 1);
			}
		}
	}
	else if (Game == 4)
	{
		if (ThePunchPlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(ThePunchPlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(ThrowPunchMappingContext);
				Subsystem->AddMappingContext(LaunchAngleMappingContext, 1);
			}
		}
	}
	else if (Game == 6)
	{
		if (ThePunchPlayerController)
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(ThePunchPlayerController->GetLocalPlayer()))
			{
				Subsystem->RemoveMappingContext(LaunchAngleMappingContext);
			}
		}
	}
}

void ACameraPawn::SetCrosshairVisibility(bool IsVisible)
{
	SpriteComp->SetVisibility(IsVisible);
}

void ACameraPawn::MoveCameraToPunchBag(float CameraDistanceFromBag)
{
	UCameraComponent* BagPawnCamera = BagPawn->GetComponentByClass<UCameraComponent>();

	FVector NewCameraLocation = BagPawn->GetSpringArmLocation();
	NewCameraLocation.Y = CameraDistanceFromBag + BagPawn->GetActorLocation().Y;

	SetActorLocationAndRotation(NewCameraLocation, BagPawnCamera->GetComponentRotation());

	FVector CrosshairLocation(CameraDistanceFromBag + BagPawn->GetActorLocation().Y - 70, 0, 0);
	SpriteComp->SetRelativeLocation(CrosshairLocation);
}

void ACameraPawn::SpawnLaunchArrow(UClass* ArrowClass, FTransform ArrowTransform)
{
	LaunchArrow = GetWorld()->SpawnActorDeferred<AScalableSprite>(ArrowClass, ArrowTransform, this);
	LaunchArrow->SetRotateSpeed(LaunchArrow->GetRotateSpeed() * (FMath::Max((SuccessfulPunchCount + ConfirmedCharge),1)));
	LaunchArrow->SetIsMovingSprite(true);
	LaunchArrow->FinishSpawning(ArrowTransform);
}

void ACameraPawn::Launch(float LaunchAngle, float LaunchPower)
{
	InitialXOffset = GetActorLocation().X;

	CapsuleComp->SetSimulatePhysics(true);
	
	FVector Impulse(LaunchPower*((100-LaunchAngle)/100), 0, LaunchPower * (LaunchAngle / 100));

	Impulse *= LaunchScalar;

	CapsuleComp->AddImpulse(Impulse);

	FVector AngularImpulse = GetActorRightVector()*AngularLaunchScalar*LaunchPower;

	CapsuleComp->AddAngularImpulseInDegrees(AngularImpulse);
}

void ACameraPawn::ThrowPunch(const FInputActionValue& Value)
{
	if (ThePunchPlayerController)
	{
		FHitResult PunchHitResult;
		ThePunchPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, PunchHitResult);

		if (ThePunchGameMode->GetPunchSpawner()->HandlePunch(&PunchHitResult))
			SuccessfulPunchCount++;
	}
}

// Called every frame
void ACameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (ThePunchGameMode->GetCurrentGame() == 1 && InputEnabled())
	{
		UpdateCharge(DeltaTime);

		for(AScalableSprite* Fist : Fists)
		{
			Fist->UpdateSpriteScale(Charge+1);
		}
	}
	else if (ThePunchGameMode->GetCurrentGame() == 2 && InputEnabled())
	{
		UpdateCrosshair(DeltaTime);
	}
}

// Called to bind functionality to input
void ACameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(BoostChargeBarAction, ETriggerEvent::Triggered, this, &ACameraPawn::BoostChargeBar);
		EnhancedInputComponent->BindAction(MoveCrosshairAction, ETriggerEvent::Triggered, this, &ACameraPawn::MoveCrosshair);
		EnhancedInputComponent->BindAction(ThrowPunchAction, ETriggerEvent::Triggered, this, &ACameraPawn::ThrowPunch);
		EnhancedInputComponent->BindAction(ConfirmLaunchAction, ETriggerEvent::Triggered, this, &ACameraPawn::ConfirmLaunchAngle);
		EnhancedInputComponent->BindAction(RestartGameAction, ETriggerEvent::Triggered, this, &ACameraPawn::RestartGame);
		EnhancedInputComponent->BindAction(QuitGameAction, ETriggerEvent::Triggered, this, &ACameraPawn::QuitGame);
	}
}

