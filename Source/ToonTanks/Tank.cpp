// Fill out your copyright notice in the Description page of Project Settings.


#include "Tank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

ATank::ATank()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ATank::BeginPlay()
{
	Super::BeginPlay();

	// This sets PlayerControllerRef = to APlayerController
	PlayerControllerRef = Cast<APlayerController>(GetController());
}

void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// When the BindAxis "MoveForward" is pressed, the function ATank::Move will be called
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ATank::Move);

	// When the BindAxis "Turn" is pressed, the function ATank::Turn will be called
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATank::Turn);

	// When the BindAction "Fire" is pressed, the funcion ATank::Fire will be called
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ATank::Fire);
}

void ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PlayerControllerRef)
	{
		FHitResult HitResult;
		// Since PlayerControllerRef is equal to APlayerController, it is able to call GetHitResultUnderCursor
		PlayerControllerRef->GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, HitResult);

		// This rotates the turret towardes the HitResult.ImpactPoint every frame
		RotateTurret(HitResult.ImpactPoint);
	}

}

// Gets called when MoveForward is pressed
void ATank::Move(float Value)
{
	//Zeros out all of the angles on the Vector
	FVector DeltaLocation = FVector::ZeroVector;
	//GetWorldDeltaSeconds is a Delta Time that required us to pass in an object from the world (we pass in "this").
	DeltaLocation.X = Value * Speed * UGameplayStatics::GetWorldDeltaSeconds(this);
	//Adds the movement and sets sweeping to true
	AddActorLocalOffset(DeltaLocation, true);
}

void ATank::Turn(float Value)
{
	// Zeros out all of the angles on the Rotator
	FRotator DeltaRotation = FRotator::ZeroRotator;

	DeltaRotation.Yaw = Value * TurnRate * UGameplayStatics::GetWorldDeltaSeconds(this);

	AddActorLocalRotation(DeltaRotation, true);
	
}