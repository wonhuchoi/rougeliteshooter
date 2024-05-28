// Fill out your copyright notice in the Description page of Project Settings.


#include "MagicProjectile.h"
#include "Components/SphereComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMagicProjectile::AMagicProjectile() : ASBaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Super::setInitialSpeed(1000.0f);
	sphereComp->OnComponentBeginOverlap.AddDynamic(this, &AMagicProjectile::OnActorOverlap);
}

void AMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AActor* test = GetInstigator();
	if (OtherActor && OtherActor != GetInstigator())
	{
			USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));

			if (AttributeComp)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), impactAudioComp->GetSound(), 1.0f, 1.0f, 0.0f);
				AttributeComp->ApplyHealthChange(GetInstigator(), -20.0f);
				Explode();
			}
	}
}
