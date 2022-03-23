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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , Category = "Track Generation")
		int LastDrawn = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , Category = "Track Generation")
		int LastRemoved = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Track Generation")
		int PlayerSplinePtPosition = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Track Generation")
		ACharacter* Player;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "Spline Piece Displacement")
		float xDisplacement = 1000.0f;

	UPROPERTY(EditAnywhere, Category = "Spline Piece Displacement")
		float yDisplacementMagnitude = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Spline Piece Displacement")
		float zDisplacement = -200.0f;

	UPROPERTY(VisibleAnywhere, Category = "Spline")
		USplineComponent* SplineComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		UStaticMesh* SplineRampMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		class UMaterialInterface* SplineMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		TEnumAsByte<ESplineMeshAxis::Type> ForwardVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline")
		int DrawAhead = 25;

	UPROPERTY(VisibleAnywhere, Category = "Track Data")
		TArray<FTrackData> SplineDataArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Obstacles")
		TSubclassOf<AActor> ObstacleBlueprint;


	void AddSplineData(int StartIndex);
	void AddSplineMeshObject(int StartIndex, int EndIndex);
	void DrawSplineBetween(int StartIndex, int EndIndex);
	FVector CalculateNewSplinePosition();
	void SetPlayerSplinePtPostion();
	UFUNCTION() void InstantiatePaintBall(FVector Location);
};