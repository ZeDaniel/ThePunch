// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ThePunchPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class THEPUNCH_API AThePunchPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	void SetPlayerEnabledState(bool bPlayerEnabled);

	void SetCursorVisibility(bool IsVisible);
	
};
