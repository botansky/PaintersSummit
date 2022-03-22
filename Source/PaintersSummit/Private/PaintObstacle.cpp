// Author: Matan Botansky


#include "PaintObstacle.h"

// Sets default values
APaintObstacle::APaintObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APaintObstacle::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APaintObstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

