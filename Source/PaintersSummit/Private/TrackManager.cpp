// Author: Matan Botansky


#include "TrackManager.h"
#include "Components/SplineMeshComponent.h"

// Sets default values
ATrackManager::ATrackManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(SplineComponent); //set up root for spline component
	SplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SplineComponent->SetMobility(EComponentMobility::Static);
}

void ATrackManager::OnConstruction(const FTransform& Transform) 
{
	//ensure spline data array and spline points are empty
	SplineDataArray.Reset();
	SplineComponent->ClearSplinePoints();

	//for each point from the player to the draw-ahead distance draw a track mesh
	int i;
	for (i = PlayerSplinePtPosition -1 ; i < DrawAhead + PlayerSplinePtPosition; i++)
	{
		AddSplineData(i + 1);

		//add a spline point at the next position
		SplineComponent->AddSplinePoint(SplineDataArray.Last().SplinePtPosition, ESplineCoordinateSpace::Local, false);
	}
	AddSplineMeshObject(PlayerSplinePtPosition - 1, i);
	LastDrawn = i;  //keep last start index drawn
}

// Called when the game starts or when spawned
void ATrackManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATrackManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATrackManager::AddSplineData(int StartIndex)
{
	FTrackData SplineData;
	SplineData.SplinePtIndex = StartIndex;
	SplineData.SplinePtPosition = CalculateNewSplinePosition();
	SplineDataArray.Add(SplineData);
}

void ATrackManager::AddSplineMeshObject(int StartIndex, int EndIndex)
{

	for (int i = StartIndex; i < EndIndex; i++) {
		if (i > 0)
		{
			FVector PreviousTangent = SplineComponent->GetLeaveTangentAtSplinePoint(i - 1, ESplineCoordinateSpace::Local);
			SplineComponent->SetTangentAtSplinePoint(i, PreviousTangent, ESplineCoordinateSpace::Local, false);
		}
		SplineComponent->SetSplinePointType(i, ESplinePointType::Curve, true);

		//create a spline mesh component
		USplineMeshComponent* SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMeshComponent->SetMobility(EComponentMobility::Static);

		//set mesh and texture, if exists
		if (SplineMesh) {

			SplineMeshComponent->SetStaticMesh(SplineMesh);
			if (SplineMaterial) {
				SplineMeshComponent->SetMaterial(0, SplineMaterial);
			}
		}

		//Set Component Details
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform); //TODO: confirm transformation rule
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); //initally disable collisions until mesh shape is set
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->SetForwardAxis(ForwardVector);


		//get start and end spline locations and tangents
		const FVector StartLoc = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndLoc = SplineComponent->GetLocationAtSplinePoint(i+1, ESplineCoordinateSpace::Local);
		const FVector StartTan = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndTan = SplineComponent->GetTangentAtSplinePoint(i+1, ESplineCoordinateSpace::Local);
		SplineMeshComponent->SetStartAndEnd(StartLoc, StartTan, EndLoc, EndTan);

		//enable collisions now that shape exists
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}

}

FVector ATrackManager::CalculateNewSplinePosition()
{
	FVector SplinePtPosition;
	if (SplineDataArray.Num() < 1) {
		SplinePtPosition = FVector(0.0f, 0.0f, 0.0f);
	}
	else
	{
		//get position last index in TrackData array (last element)
		FVector LastPosition = SplineDataArray.Last().SplinePtPosition;

		//apply displacement to new position
		SplinePtPosition = FVector(LastPosition.X + xDisplacement,
			LastPosition.Y + (rand() % (int)(2.0f * yDisplacementMagnitude)) - yDisplacementMagnitude, LastPosition.Z + zDisplacement);
	}

	return SplinePtPosition;
}


