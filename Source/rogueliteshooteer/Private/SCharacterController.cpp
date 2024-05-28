// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacterController.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

// Called when this player controller possesses its pawn
void ASCharacterController::OnPossess(APawn* InPawn)
{
	// We need to execute the parent OnPossess's functionality like this:
	Super::OnPossess(InPawn);

	// Use the possessed pawn to get and store a reference to our player character
	PlayerCharacter = Cast<ASCharacter>(InPawn);

	// Log details and crash the game if the player character reference was not set
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Fatal, TEXT("Player Character Cast Failed"))
	}

	// Add the input mapping context by accessing the enhanced input subsystem
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// If the subsytem was retrieved, add default mapping context
		Subsystem->AddMappingContext(DefaultMappingContxt, 0);
	}

	// Print error if the susbsystem was not retrieved
	else
	{
		// pass
	}
}

// Executes functionality from player character to move that character based on value of input
void ASCharacterController::OnMovePlayer(FInputActionValue& Value)
{
	// Convert input value to vector
	FVector2D InputVector = Value.Get<FVector2D>();

	// Normalize vector to prevent diagonal movement being faster
	InputVector.Normalize();

	// Execute functionality from player character
	PlayerCharacter->Move(InputVector);
}

// Rotates this controller based on value of input
void ASCharacterController::Rotate(FInputActionValue& Value)
{
	// Convert input value to vector
	FVector2D InputVector = Value.Get<FVector2D>();

	if (InputVector.X != 0 || InputVector.Y != 0)
	{
		// Add rotation to controller
		AddPitchInput(InputVector.Y);
		AddYawInput(InputVector.X);
	}
}