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
        void StartFlocking(AActor* leader, float rowDistance, float rowSpacing, TArray<AActor*> flockMembers, USplineComponent* splineComponent, bool isPlayer, bool playerFollowLeader);

    UFUNCTION(BlueprintCallable, Category = "Flocking")
        void Flock(float deltaTime);
    UFUNCTION(BlueprintCallable, Category = "Flocking")
        void AddSmoothLeaderRotationQuaternion(FQuat targetQuat, float rotationSpeed, float rotationStopDistance);
    UFUNCTION(BlueprintCallable, Category = "Flocking")
        void ChangePlayerSpeed();


private:
    bool _isFlocking;
    bool _leaderRotating;
    bool _isPlayer;
    bool _playerFollowLeader;
    AActor* _leader;
    float _rowDistance;
    float _rowSpacing;
    float _leaderRotationSpeed;
    float _rotationStopDistance;
    float _cachedMaxSpeed;
    FQuat _targetLeaderQuat;

    TArray<AActor*> _flockMembers;
    USplineComponent* _splineComponent;
    UFloatingPawnMovement* _movementComponent;
};
