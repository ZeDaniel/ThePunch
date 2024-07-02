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
			Subsystem->AddMappingContext(BoostBarMappingContext, 0);
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

float ACameraPawn::GetCurrentAccuracy()
{
	FVector CrosshairLocation = SpriteComp->GetComponentLocation();
	FVector BagLocation = BagPawn->GetSpringArmLocation();
	CrosshairLocation.Y = BagLocation.Y;
	
	float DistanceFromTarget = FVector::Distance(CrosshairLocation, BagLocation);
	//return DistanceFromTarget;
	return FMath::Max(MaxAccuracyScore - DistanceFromTarget / AccuracyScoreDivider, 0.1);
}

FVector ACameraPawn::GetSpringArmLocation()
{
	return SpringArmComp->GetComponentLocation();
}

void ACameraPawn::ConfirmCharge()
{
	ConfirmedCharge = FMath::Min(Charge, MaxCharge);
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
				Subsystem->AddMappingContext(AimCrosshairMappingContext, 0);
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
				Subsystem->AddMappingContext(ThrowPunchMappingContext, 0);
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

void ACameraPawn::ThrowPunch(const FInputActionValue& Value)
{
	if (ThePunchPlayerController)
	{
		FHitResult PunchHitResult;
		ThePunchPlayerController->GetHitResultUnderCursor(ECollisionChannel::ECC_Destructible, false, PunchHitResult);

		ThePunchGameMode->GetPunchSpawner()->HandlePunch(&PunchHitResult);
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
	}
}

