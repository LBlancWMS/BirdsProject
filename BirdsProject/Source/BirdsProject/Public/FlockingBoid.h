#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "FlockingBoid.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BIRDSPROJECT_API UFlockingBoid : public UActorComponent
{
    GENERATED_BODY()

public:
    UFlockingBoid();

    UFUNCTION(BlueprintCallable, Category = "Flocking")
        void StartFlocking(AActor* leader, float rowDistance, float rowSpacing, TArray<AActor*> flockMembers, USplineComponent* splineComponent);

    UFUNCTION(BlueprintCallable, Category = "Flocking")
        void Flock(float deltaTime);

private:
    bool _isFlocking;
    AActor* _leader;
    float _rowDistance;
    float _rowSpacing;

    TArray<AActor*> _flockMembers;
    USplineComponent* _splineComponent;
    UFloatingPawnMovement* _movementComponent;
};
