// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackGenerator.generated.h"

USTRUCT(BlueprintType)
struct FTrackData 
{
	GENERATED_BODY()

	FTrackData() {
		UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FVector position;
	}
};

UCLASS()
class PAINTERSSUMMIT_API ATrackGenerator : public AActor
{
	GENERATED_BODY()
	
public:	

	UPROPERTY(EditAnywhere, Category = "Spline Piece Displacement") float xDisplacement = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Spline Piece Displacement") float yDisplacementMagnitude;
	UPROPERTY(EditAnywhere, Category = "Spline Piece Displacement") float zDisplacement = -100.0f;
	ATrackGenerator();

protected:

	UPROPERTY(VisibleAnywhere, Category = "Track Data") TArray<FTrackData> trackDataArray;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AddSplineMeshObject(int startIndex);

};
