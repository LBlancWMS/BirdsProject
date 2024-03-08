#include "FlockingBoid.h"
#include "Kismet/KismetMathLibrary.h"


UFlockingBoid::UFlockingBoid()
{
    PrimaryComponentTick.bCanEverTick = false;
    _isFlocking = false;
    _leaderRotating = false;
    _isPlayer = false;
    _playerFollowLeader = false;
}

void UFlockingBoid::StartFlocking(AActor* leader, float rowDistance, float rowSpacing, TArray<AActor*> flockMembers, USplineComponent* splineComponent, bool isPlayer, bool playerFollowLeader)
{
    _leader = leader;
    _isPlayer = isPlayer;
    _playerFollowLeader = playerFollowLeader;
    _rowDistance = rowDistance;
    _rowSpacing = rowSpacing;
    _flockMembers = flockMembers;
    _splineComponent = splineComponent;
    _movementComponent = GetOwner()->FindComponentByClass<UFloatingPawnMovement>();
    _cachedMaxSpeed = _movementComponent->MaxSpeed;
    _isFlocking = true;
}

void UFlockingBoid::AddSmoothLeaderRotationQuaternion(FQuat targetQuat, float rotationSpeed, float rotationStopDistance)
{
    //TODO : custom le multiplicateur de vitesse et donc changer le système d'animation / speed
    if (_isFlocking)
    {
        _targetLeaderQuat = targetQuat;
        _leaderRotationSpeed = rotationSpeed;
        _rotationStopDistance = rotationStopDistance;
        _movementComponent->MaxSpeed = 4800.0f;
        _leaderRotating = true;
    }
}

void UFlockingBoid::Flock(float deltaTime)
{
    // TODO : faire 2 fonctions au lieu d'une
    if (_isFlocking)
    {
        FVector MoveDirection = GetOwner()->GetActorForwardVector();
        _movementComponent->AddInputVector(MoveDirection * _movementComponent->MaxSpeed);

        if (GetOwner() == _leader && _leaderRotating)
        {
            FQuat currentRotation = FQuat(GetOwner()->GetActorRotation());
            FQuat newRotation = FQuat::Slerp(currentRotation, currentRotation * _targetLeaderQuat, deltaTime * _leaderRotationSpeed);
            GetOwner()->SetActorRotation(newRotation.Rotator());
            if (currentRotation.AngularDistance(_targetLeaderQuat) <= _rotationStopDistance)
            {
                _movementComponent->MaxSpeed = 2400.0f;
                _leaderRotating = false;
            }
        }
        else if (!_isPlayer || _isPlayer && _playerFollowLeader)
        {
                //TODO : opti, remplacer les var locales par une struct et réduire le nombre de calculs si possible
                _movementComponent->MaxSpeed = _leader->FindComponentByClass<UFloatingPawnMovement>()->MaxSpeed;
                FVector directionToLeader = (_leader->GetActorLocation() - GetOwner()->GetActorLocation()).GetSafeNormal();
                FQuat LookAtRotation = FQuat::FindBetweenNormals(FVector::ForwardVector, directionToLeader);
                FQuat CurrentRotation = FQuat(GetOwner()->GetActorRotation());
                FQuat NewRotation = FQuat::Slerp(CurrentRotation, LookAtRotation, deltaTime * 2.0f);
                float DistanceToLeader = FVector::Distance(GetOwner()->GetActorLocation(), _leader->GetActorLocation());
                float OffsetFactor = FMath::Clamp(DistanceToLeader / _rowDistance, 0.0f, 1.0f);
                FVector Offset = GetOwner()->GetActorRightVector() * (_rowSpacing * OffsetFactor);
                FQuat OffsetRotation = FQuat::MakeFromEuler(Offset);
                NewRotation = NewRotation * OffsetRotation;

                GetOwner()->SetActorRotation(NewRotation.Rotator());
        }
    }
}
