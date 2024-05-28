// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "SInteractionsComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "SAttributeComponent.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionsComponent>("InteractionComp");
	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttributeComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	TimeToHitParamName = "TimeToHit";
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UCameraComponent* ASCharacter::GetCameraComp()
{
	return CameraComp;
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Pitch", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("BlackHoleAttack", IE_Pressed, this, &ASCharacter::BlackHoleAttack);
	PlayerInputComponent->BindAction("TeleportProjectileAttack", IE_Pressed, this, &ASCharacter::TeleportProjectileAttack);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	HandSocketName = "Muzzle_01";
}


void ASCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASCharacter::OnHealthChanged);
}

void ASCharacter::MoveForward(float value)
{
	FRotator controlRot = GetControlRotation();
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;
	AddMovementInput(controlRot.Vector(), value);
}

void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}


void ASCharacter::PrimaryAttack_TimeElapsed()
{
	FireProjectile(ProjectileClass);
}

void ASCharacter::BlackHoleAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::BlackHoleAttack_TimeElapsed, 0.2f);
}


void ASCharacter::BlackHoleAttack_TimeElapsed()
{
	FireProjectile(BlackHoleClass);
}

void ASCharacter::TeleportProjectileAttack()
{
	PlayAnimMontage(AttackAnim);
	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::TeleportProjectileAttack_TimeElapsed, 0.2f);
}


void ASCharacter::TeleportProjectileAttack_TimeElapsed()
{
	FireProjectile(TeleportClass);
}

void ASCharacter::FireProjectile(TSubclassOf<ASBaseProjectile> ProjectileSubtype)
{
	if (ensureAlways(ProjectileSubtype))
	{
		FVector EyeLocation = CameraComp->GetComponentLocation();
		FRotator EyeRotation = CameraComp->GetComponentRotation();
		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		float Radius = 20.0f;

		FCollisionShape Shape;
		Shape.SetSphere(Radius);

		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);

		FVector handLocation = GetMesh()->GetSocketLocation(HandSocketName);
		FHitResult Hit;
		FVector End = EyeLocation + (EyeRotation.Vector() * 5000);
		bool bActorHit = GetWorld()->SweepSingleByObjectType(Hit, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Shape, Params);
		FVector ToVec;
		AActor* HitActor = Hit.GetActor();

		if (bActorHit)
		{
			ToVec = Hit.ImpactPoint;
		}
		else
		{
			ToVec = End;
		}

		FRotator ToRotator = FRotationMatrix::MakeFromX(ToVec - handLocation).Rotator();
		FTransform SpawnTM = FTransform(ToRotator, handLocation);

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		GetWorld()->SpawnActor<AActor>(ProjectileSubtype, SpawnTM, SpawnParams);
	}
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

void ASCharacter::MoveRight(float value)
{
	FRotator controlRot = GetControlRotation();
	FVector rightVec = FRotationMatrix(controlRot).GetScaledAxis(EAxis::Y);
	controlRot.Pitch = 0.0f;
	controlRot.Roll = 0.0f;
	AddMovementInput(rightVec, value);
}

void ASCharacter::Move(const FVector2D InputVector)
{
	// Store the yaw rotation of this character's controller
	const FRotator YawRotation = FRotator(0, GetControlRotation().Yaw, 0);

	// Determine if there is input to move character on X axis (forward / backward)
	if (InputVector.Y != 0)
	{
		// Use FRotationMatrix to find forward direction relative to controller's yaw rotation
		const FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// Add movement input based on this direction and Y value stored in input vector
		AddMovementInput(ForwardVector, InputVector.Y);
	}

	// Determine if there is input to move character on Y axis (right / left)
	if (InputVector.X != 0)
	{
		// Use FRotationMatrix to find right direction relative to controller's yaw rotation
		const FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement input based on this direction and X value stored in input vector
		AddMovementInput(RightVector, InputVector.X);
	}
}

void ASCharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	// Damaged
	if (Delta < 0.0f)
	{
		// Replaces the above "old" method of requiring unique material instances for every mesh element on the player 
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->TimeSeconds);

	}

	if (NewHealth <= 0.0f && Delta < 0.0f)
	{
		APlayerController* PC = Cast<APlayerController>(GetController());
		DisableInput(PC);
	}
}

void ASCharacter::HealSelf(float Amount)
{
	AttributeComp->ApplyHealthChange(this, Amount);
}