// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosiveBarrel.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
AExplosiveBarrel::AExplosiveBarrel()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	staticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>("staticMeshComp");
	staticMeshComp->SetupAttachment(RootComponent);

	radialForceComp = CreateDefaultSubobject<URadialForceComponent>("radialForceComp");
	radialForceComp->SetupAttachment(staticMeshComp);
	radialForceComp->Radius = 600.0f;
	radialForceComp->ImpulseStrength = 2000.0f;
	radialForceComp->bImpulseVelChange = true;

	staticMeshComp->SetCollisionProfileName("Breakable");
	staticMeshComp->SetSimulatePhysics(true);
	staticMeshComp->OnComponentBeginOverlap.AddDynamic(this, &AExplosiveBarrel::Explode);

}

// Called when the game starts or when spawned
void AExplosiveBarrel::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AExplosiveBarrel::Explode(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	radialForceComp->FireImpulse();
}

// Called every frame
void AExplosiveBarrel::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

