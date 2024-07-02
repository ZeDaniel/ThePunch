// Fill out your copyright notice in the Description page of Project Settings.


#include "ThePunchPlayerController.h"

void AThePunchPlayerController::SetPlayerEnabledState(bool bPlayerEnabled)
{
	if (bPlayerEnabled)
	{
		GetPawn()->EnableInput(this);
	}
	else
	{
		GetPawn()->DisableInput(this);
	}
}

void AThePunchPlayerController::SetCursorVisibility(bool IsVisible)
{
	bShowMouseCursor = IsVisible;
}
