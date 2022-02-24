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
	SplineComponent->SetMobility(EComponentMobility::Static);

	FTrackData InitTrackData;
	InitTrackData.SplinePtIndex = 0;
	InitTrackData.Drawn = false;
	InitTrackData.SplinePtPosition = this->GetTransform().GetLocation();
	SplineDataArray.Add(InitTrackData);
}

void ATrackManager::OnConstruction(const FTransform& Transform) {

	//default second spline point position
	SplineComponent->SetLocationAtSplinePoint(1, CalculateNewSplinePosition(), ESplineCoordinateSpace::Local, true);

	//loop through already constructed spline points and draw mesh components
	int PtCount = SplineComponent->GetNumberOfSplinePoints() - 1;
	for (int ptInd = 0; ptInd < PtCount; ptInd++) {
		AddSplineMeshObject(ptInd);
	}
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

void ATrackManager::AddSplineMeshObject(int StartIndex)
{
	int NextIndex = StartIndex + 1;

	//update spline index and positon of next spline point
	int EndIndex = StartIndex + 1;
	SplineComponent->SetLocationAtSplinePoint(EndIndex, CalculateNewSplinePosition(), ESplineCoordinateSpace::Local, true);

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
	const FVector StartLoc = SplineComponent->GetLocationAtSplinePoint(StartIndex, ESplineCoordinateSpace::Local);
	const FVector EndLoc = SplineComponent->GetLocationAtSplinePoint(EndIndex, ESplineCoordinateSpace::Local);
	const FVector StartTan = SplineComponent->GetTangentAtSplinePoint(StartIndex, ESplineCoordinateSpace::Local);
	const FVector EndTan = SplineComponent->GetTangentAtSplinePoint(EndIndex, ESplineCoordinateSpace::Local);
	SplineMeshComponent->SetStartAndEnd(StartLoc, StartTan, EndLoc, EndTan);

	//enable collisions now that shape exists
	SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	//add component to track data array
	FTrackData TrackData;
	TrackData.SplinePtIndex = StartIndex;
	TrackData.Drawn = true;
	TrackData.SplinePtPosition = StartLoc;
	SplineDataArray.Add(TrackData);
}

FVector ATrackManager::CalculateNewSplinePosition()
{
	//get position last index in TrackData array (last element)
	FVector LastPosition = SplineDataArray.Last().SplinePtPosition;

	//apply displacement to new position
	FVector NewPosition = FVector(LastPosition.X + xDisplacement,
		LastPosition.Y + (rand() % 2 * yDisplacementMagnitude) - yDisplacementMagnitude, LastPosition.Z + zDisplacement);

	return NewPosition;
}


