// Fill out your copyright notice in the Description page of Project Settings.


#include "Ai/SBTService_CheckLowHealth.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "SAttributeComponent.h"

USBTService_CheckLowHealth::USBTService_CheckLowHealth()
{
	LowHealthThreshhold = 0.3f;
}

void USBTService_CheckLowHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if (ensure(BlackboardComp))
	{
		APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributes(AIPawn);		
		if (ensure(AttributeComp))
		{
			bool bLowHealth = (AttributeComp->GetHealth() / AttributeComp->GetMaxHealth()) < LowHealthThreshhold;
			BlackboardComp->SetValueAsBool(IsLowHealthKey.SelectedKeyName, bLowHealth);
		}
	}
}