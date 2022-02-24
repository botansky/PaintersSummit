// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "TrackGenerator.generated.h"

USTRUCT(BlueprintType)
struct FTrackData 
{
	GENERATED_BODY()

	FTrackData() {
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FVector SplinePtPosition;
	}
};

UCLASS()
class PAINTERSSUMMIT_API ATrackGenerator : public AActor
{
	GENERATED_BODY()
	
public:	

	ATrackGenerator();

	//enable custom construction script
	void OnConstruction(const FTransform& Transform) override;

protected:

	//UPROPERTY(VisibleAnywhere, Category = "Track Data") TArray<FTrackData> trackDataArray;

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

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Spline") 
	UStaticMesh* SplineMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline") 
	class UMaterialInterface* SplineMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spline") 
	TEnumAsByte<ESplineMeshAxis::Type> ForwardVector;

	void AddSplineMeshObject(int startIndex);
};
