// Fill out your copyright notice in the Description page of Project Settings.


#include "ScalableSprite.h"
#include "PaperSpriteComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AScalableSprite::AScalableSprite()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Component"));
	RootComponent = BoxComp;

	SpriteComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	SpriteComp->SetupAttachment(BoxComp);

}

void AScalableSprite::UpdateSpriteScale(float NewSpriteScale)
{
	SpriteScale = NewSpriteScale;

	FVector ActorScale(SpriteScale, 1, SpriteScale);

	SetActorScale3D(ActorScale);
}

void AScalableSprite::HandleDestruction()
{
	//sound effect and anim
	BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SpriteComp->SetVisibility(false);

	if(SpriteDeathSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpriteDeathSound, GetActorLocation());
}

void AScalableSprite::SetSpriteColor(FColor NewColor)
{
	SpriteComp->SetSpriteColor(NewColor);
}

// Called when the game starts or when spawned
void AScalableSprite::BeginPlay()
{
	Super::BeginPlay();

	if (SpriteSpawnSound)
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), SpriteSpawnSound, GetActorLocation());
}

// Called every frame
void AScalableSprite::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsMovingSprite)
	{
		if (RootComponent->GetRelativeRotation().Pitch >= RotateMax)
		{
			RotateSpeed = -RotateSpeed;
		}
		else if (RootComponent->GetRelativeRotation().Pitch <= RotateMin)
		{
			RotateSpeed = -RotateSpeed;
		}

		FRotator DeltaRotation(DeltaTime * RotateSpeed, 0, 0);
		RootComponent->AddLocalRotation(DeltaRotation);
	}
}

