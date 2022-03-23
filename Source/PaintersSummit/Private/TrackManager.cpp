// Author: Matan Botansky

#include "TrackManager.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ATrackManager::ATrackManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SplineComponent = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(SplineComponent); //set up root for spline component
	SplineComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	SplineComponent->SetMobility(EComponentMobility::Static);

	//get player object reference
	Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
}

void ATrackManager::OnConstruction(const FTransform& Transform) 
{
	//ensure spline data array and spline points are empty
	SplineDataArray.Reset();
	SplineComponent->ClearSplinePoints();
	//draw first section of spline
	DrawSplineBetween(PlayerSplinePtPosition - 1, DrawAhead + PlayerSplinePtPosition);
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
	SetPlayerSplinePtPostion();
	if (LastDrawn - PlayerSplinePtPosition == 5) {
		DrawSplineBetween(LastDrawn, LastDrawn + DrawAhead);
	}
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
		if (SplineMesh && SplineRampMesh) {
			if (i % 8 == 0 && i != 0) {
				SplineMeshComponent->SetStaticMesh(SplineRampMesh);
			}
			else
			{
				SplineMeshComponent->SetStaticMesh(SplineMesh);
			}
			if (SplineMaterial) {
				SplineMeshComponent->SetMaterial(0, SplineMaterial);
			}
		}

		//Set Component Details
		SplineMeshComponent->AttachToComponent(SplineComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); //initally disable collisions until mesh shape is set
		SplineMeshComponent->CreationMethod = EComponentCreationMethod::UserConstructionScript;
		SplineMeshComponent->RegisterComponentWithWorld(GetWorld());
		SplineMeshComponent->SetForwardAxis(ForwardVector);

		//get start and end spline locations and tangents
		const FVector StartLoc = SplineComponent->GetLocationAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndLoc = SplineComponent->GetLocationAtSplinePoint(i+1, ESplineCoordinateSpace::Local);
		const FVector StartTan = SplineComponent->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
		const FVector EndTan = SplineComponent->GetTangentAtSplinePoint(i+1, ESplineCoordinateSpace::Local);
		SplineMeshComponent->SetStartAndEnd(StartLoc, StartTan, EndLoc, EndTan);

		if (i % 2 == 1 && i != 1 && i > 4) { //spawn obstacle
			FVector ObjectTargetLocation = (StartLoc + EndLoc) * 0.5f;
			InstantiatePaintBall(ObjectTargetLocation);
		}

		//enable collisions now that shape exists
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
}

void ATrackManager::DrawSplineBetween(int StartIndex, int EndIndex)
{
	//for each point from the player to the draw-ahead distance draw a track mesh
	int i;
	for (i = StartIndex - 1; i < EndIndex; i++)
	{
		AddSplineData(i + 1);

		//add a spline point at the next position
		SplineComponent->AddSplinePoint(SplineDataArray.Last().SplinePtPosition, ESplineCoordinateSpace::Local, false);
	}

	AddSplineMeshObject(StartIndex, EndIndex);
	LastDrawn = i;  //keep last start index drawn

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
		SplinePtPosition = FVector(
			LastPosition.X + xDisplacement, 
			LastPosition.Y + (rand() % (int)(2.0f * yDisplacementMagnitude)) - yDisplacementMagnitude, 
			LastPosition.Z + zDisplacement 
		);
	}

	return SplinePtPosition;
}

void ATrackManager::SetPlayerSplinePtPostion()
{
	PlayerSplinePtPosition = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation().X / xDisplacement;
}

void ATrackManager::InstantiatePaintBall(FVector Location)
{
	FActorSpawnParameters SpawnParameters;
	AActor* ActorRef = GetWorld()->SpawnActor<AActor>(ObstacleBlueprint);
	ActorRef->SetActorLocation(Location);
}
