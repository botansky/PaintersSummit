// Author: Matan Botansky


#include "BoardController.h"

// Sets default values
ABoardController::ABoardController()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

// Called when the game starts or when spawned
void ABoardController::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABoardController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForward();
}

// Called to bind functionality to input
void ABoardController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("Horizontal", this, &ABoardController::MoveHorizontal);
}

void ABoardController::MoveHorizontal(float HorizontalMovement)
{
	if (HorizontalMovement && HorizontalMovement != 0.0f) {
		AddMovementInput(GetActorRightVector(), HorizontalMovement * HorizontalMovementBaseSpeed);
	}
}

void ABoardController::MoveForward()
{
	AddMovementInput(GetActorForwardVector(), (ForwardMovementBaseSpeed));
}

FVector ABoardController::GetPlayerPosition()
{
	return this->GetTransform().GetLocation();
}
