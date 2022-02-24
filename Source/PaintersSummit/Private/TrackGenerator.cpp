// Fill out your copyright notice in the Description page of Project Settings.


#include "TrackGenerator.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
ATrackGenerator::ATrackGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(SplineComponent); //set up root for spline component
	SplineComponent->SetMobility(EComponentMobility::Static);
}

void ATrackGenerator::OnConstruction(const FTransform& Transform) {

	//loop through already constructed spline points and draw mesh components
	int PtCount = SplineComponent->GetNumberOfSplinePoints() - 1;
	for (int ptInd = 0; ptInd < PtCount; ptInd++) {
		AddSplineMeshObject(ptInd);
	}
}

// Called when the game starts or when spawned
void ATrackGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrackGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrackGenerator::AddSplineMeshObject(int StartIndex)
{
	if (SplineMesh) {
		int EndIndex = StartIndex+1;

		//create a spline mesh component
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetMobility(EComponentMobility::Static);
		SplineMeshComponent->SetStaticMesh(SplineMesh);
		if (SplineMaterial) {
			SplineMeshComponent->SetMaterial(0, SplineMaterial);
		}
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform); //TODO: confirm transformation rule
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); //initally disable collisions until mesh shape is set
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->SetForwardAxis(ForwardVector);


		//get start and end spline locations and tangents
		const FVector StartLoc = SplineComponent->GetLocationAtSplinePoint(StartIndex, ESplineCoordinateSpace::Local);
		const FVector EndLoc = SplineComponent->GetLocationAtSplinePoint(EndIndex, ESplineCoordinateSpace::Local);
		const FVector StartTan = SplineComponent->GetTangentAtSplinePoint(StartIndex, ESplineCoordinateSpace::Local);
		const FVector EndTan = SplineComponent->GetTangentAtSplinePoint(EndIndex, ESplineCoordinateSpace::Local);
		SplineMeshComponent->SetStartAndEnd(StartLoc, StartTan, EndLoc, EndTan);

		//enable collisions now that shape exists
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}


