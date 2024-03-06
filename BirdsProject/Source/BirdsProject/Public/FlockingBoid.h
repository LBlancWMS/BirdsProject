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
        void StartFlocking(AActor* leader, float rowDistance, float rowSpacing, float verticalSpacing, TArray<AActor*> flockMembers, float flockDelay, float flockOffset, USplineComponent* splineComponent);

    UFUNCTION(BlueprintCallable, Category = "Flocking")
        void Flock(float deltaTime);

private:
    bool _isFlocking;

    AActor* _leader;
    float _rowDistance;
    float _rowSpacing;
    float _verticalSpacing;
    float _flockDelay;
    float _flockOffset;
    float _splineInterpTime;
    float _splineInterpSpeed;
    float _randomRange;

    TArray<AActor*> _flockMembers;
    USplineComponent* _splineComponent;
    UFloatingPawnMovement* _movementComponent;
};
