// Author: Matan Botansky

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/InputComponent.h"
#include "BoardController.generated.h"

UCLASS()
class PAINTERSSUMMIT_API ABoardController : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABoardController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
		float ForwardMovementBaseSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
		float HorizontalMovementBaseSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isAlive = true;
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:
	void MoveHorizontal(float HorizontalMovement);
	void MoveForward();

public:
	FVector GetPlayerPosition();
	void GameOver();
};
