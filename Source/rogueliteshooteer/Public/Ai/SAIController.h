// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "SAIController.generated.h"

class UBehaviorTree;

/**
 * 
 */
UCLASS()
class ROGUELITESHOOTEER_API ASAIController : public AAIController
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Ai")
	UBehaviorTree* BehaviorTree;
	virtual void BeginPlay() override;
};
