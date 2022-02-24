// Author: Matan Botansky

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "TrackManager.generated.h"

USTRUCT(BlueprintType)
struct FTrackData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 SplinePtIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) bool Drawn = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FVector SplinePtPosition;
};

UCLASS()
class PAINTERSSUMMIT_API ATrackManager : public AActor
{
	GENERATED_BODY()

public:

	ATrackManager();

	//enable custom construction script
	void OnConstruction(const FTransform& Transform) override;

protected:

	UPROPERTY(VisibleAnywhere, Category = "Track Data") TArray<FTrackData> trackDataArray;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "Spline Piece Displacement")
		float xDisplacement = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Spline Piece Displacement")
		float yDisplacementMagnitude;

	UPROPERTY(EditAnywhere, Category = "Spline Piece Displacement")
		float zDisplacement = -100.0f;

	UPROPERTY(VisibleAnywhere, Category = "Spline")
		USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		class UMaterialInterface* SplineMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		TEnumAsByte<ESplineMeshAxis::Type> ForwardVector;

	UPROPERTY(VisibleAnywhere, Category = "Track Data")
		TArray<FTrackData> SplineDataArray;

	void AddSplineMeshObject(int startIndex);
	FVector CalculateNewSplinePosition();
};