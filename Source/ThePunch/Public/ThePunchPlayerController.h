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

	UFUNCTION(BlueprintImplementableEvent)
	void RunGame1UI();

	UFUNCTION(BlueprintImplementableEvent)
	void RunGame2UI();

	UFUNCTION(BlueprintImplementableEvent)
	void RunGame3UI();

	UFUNCTION(BlueprintImplementableEvent)
	void RunGame4UI();

	UFUNCTION(BlueprintImplementableEvent)
	void RunEndGameUI();
};
