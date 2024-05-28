// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/SAIController.h"
#include "Kismet/Gameplaystatics.h"
#include "BehaviorTree/BlackboardComponent.h"

// Called when the game starts or when spawned
void ASAIController::BeginPlay()
{
	Super::BeginPlay();
	if (ensureMsgf(BehaviorTree, TEXT("Behavior Tree is nullptr! Please assign BehaviorTree in your AI controller"))) {
		RunBehaviorTree(BehaviorTree);
	}

}