// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/SBTTask_HealSelf.h"
#include "AIController.h"
#include "GameFramework/Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "SAttributeComponent.h"

USBTTask_HealSelf::USBTTask_HealSelf()
{
	HealthRegen = 100.0f;
}


EBTNodeResult::Type USBTTask_HealSelf::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* MyController = OwnerComp.GetAIOwner();

	if (ensure(MyController))
	{
		ACharacter* MyPawn = Cast<ACharacter>(MyController->GetPawn());
		if (MyPawn == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		USAttributeComponent* Attributes = USAttributeComponent::GetAttributes(MyPawn);
		bool Healed = Attributes->ApplyHealthChange(MyPawn, HealthRegen);
		return Healed ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
