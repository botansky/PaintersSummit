// Author: Matan Botansky


#include "BoardController.h"
#include "Components/CapsuleComponent.h"
#include "TrackManager.h"
#include "Kismet/GameplayStatics.h"

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
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ABoardController::OnOverlapBegin);
}

// Called every frame
void ABoardController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MoveForward();

	FVector CurrentPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	float PlayerMinZPos = ((int)(CurrentPos.X / 1000) * -500.0f) - 500.0f;
	if (CurrentPos.Z < PlayerMinZPos) {
		GameOver();
	}
}

// Called to bind functionality to input
void ABoardController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("Horizontal", this, &ABoardController::MoveHorizontal);
}

void ABoardController::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA(ATrackManager::StaticClass())) {
		GameOver();
	}

	//FTimerHandle NewTimerHandle;
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

void ABoardController::GameOver()
{
	isAlive = false;
}
