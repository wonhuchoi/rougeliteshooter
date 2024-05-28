// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SCharacter.h"
#include "GameFramework/PlayerController.h"
#include "SCharacterController.generated.h"

struct FInputActionValue;
class UInputAction;
/**
 * 
 */
UCLASS()
class ROGUELITESHOOTEER_API ASCharacterController : public APlayerController
{
	GENERATED_BODY()
	// Under private member functions section:
	public:
		// Executes functionality from player character class to move this character based on value of input
		void OnMovePlayer(FInputActionValue& Value);

		// Rotates this controller based on the value of input
		void Rotate(FInputActionValue& Value);
	private:
		// Stores the mapping context that holds our input action bindings
		UPROPERTY(EditDefaultsOnly, Category = "Player | Input")
		TObjectPtr<class UInputMappingContext> DefaultMappingContxt = nullptr;

		// Pointers to our input actions, which will be bound to our input functionality
		UPROPERTY(EditDefaultsOnly, Category = "Player | Input")
		TObjectPtr<UInputAction> Activate_Action = nullptr;

		UPROPERTY(EditDefaultsOnly, Category = "Player | Input")
		TObjectPtr<UInputAction> Crouch_Action = nullptr;

		UPROPERTY(EditDefaultsOnly, Category = "Player | Input")
		TObjectPtr<UInputAction> Jump_Action = nullptr;

		UPROPERTY(EditDefaultsOnly, Category = "Player | Input")
		TObjectPtr<UInputAction> Move_Action = nullptr;

		UPROPERTY(EditDefaultsOnly, Category = "Player | Input")
		TObjectPtr<UInputAction> Rotate_Action = nullptr;

		UPROPERTY(EditDefaultsOnly, Category = "Player | Input")
		TObjectPtr<UInputAction> Sprint_Action = nullptr;		

		// Called when this controller possesses its pawn
		virtual void OnPossess(APawn* InPawn) override;

		// Stores a reference to our player character class
		// It's important we name this pointer something other than Player, as this class already has a variable with that name
		UPROPERTY()
		TObjectPtr<class ASCharacter> PlayerCharacter = nullptr;
};
