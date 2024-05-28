// Fill out your copyright notice in the Description page of Project Settings.


#include "SBaseProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASBaseProjectile::ASBaseProjectile()
{
	sphereComp = CreateDefaultSubobject<USphereComponent>("sphereComp");
	sphereComp->SetCollisionProfileName("Projectile");
	sphereComp->OnComponentHit.AddDynamic(this, &ASBaseProjectile::OnActorHit);
	RootComponent = sphereComp;

	effectComp = CreateDefaultSubobject<UParticleSystemComponent>("effectComp");
	effectComp->SetupAttachment(sphereComp);

	audioComp = CreateDefaultSubobject<UAudioComponent>("audioComp");
	audioComp->SetupAttachment(sphereComp);

	impactAudioComp = CreateDefaultSubobject<UAudioComponent>("impactAudioComp");
	impactAudioComp->SetupAttachment(sphereComp);

	movementComp = CreateDefaultSubobject<UProjectileMovementComponent>("movementComp");
	movementComp->InitialSpeed = 1000.f;
	movementComp->bRotationFollowsVelocity = true;
	movementComp->bInitialVelocityInLocalSpace = true;
	
	audioComp->Play();
}

void ASBaseProjectile::setInitialSpeed(float speed)
{
	this->initialSpeed = speed;
}

void ASBaseProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}

void ASBaseProjectile::Explode_Implementation()
{
	if (ensure(IsValid(this)))
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());
		impactAudioComp->Play();
		Destroy();
	}
}

// Called when the game starts or when spawned
void ASBaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASBaseProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}
