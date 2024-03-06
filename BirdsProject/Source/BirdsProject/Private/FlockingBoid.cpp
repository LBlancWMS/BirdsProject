#include "FlockingBoid.h"
#include "Kismet/KismetMathLibrary.h"

UFlockingBoid::UFlockingBoid()
{
    PrimaryComponentTick.bCanEverTick = false;
    _isFlocking = false;
}

void UFlockingBoid::StartFlocking(AActor* leader, float rowDistance, float rowSpacing, TArray<AActor*> flockMembers, USplineComponent* splineComponent)
{
    _leader = leader;
    _rowDistance = rowDistance;
    _rowSpacing = rowSpacing;
    _flockMembers = flockMembers;
    _splineComponent = splineComponent;
    _movementComponent = GetOwner()->FindComponentByClass<UFloatingPawnMovement>();
    _isFlocking = true;
}

void UFlockingBoid::Flock(float deltaTime)
{
    if (_isFlocking)
    {
        FVector MoveDirection = GetOwner()->GetActorForwardVector();
        _movementComponent->AddInputVector(MoveDirection * _movementComponent->MaxSpeed);

        if (GetOwner() == _leader)
        {
            FQuat currentRotation = FQuat(GetOwner()->GetActorRotation());
            FRotator randomRotationAngles(FMath::FRandRange(-90.0f, 90.0f), 0.0f, 0.0f);
            FQuat randomRotation = FQuat(randomRotationAngles);
            FQuat newRotation = FQuat::Slerp(currentRotation, currentRotation * randomRotation, FMath::Clamp(deltaTime * 2.0f, 0.0f, 1.0f));
            GetOwner()->SetActorRotation(newRotation.Rotator());
        }
        else if (_leader)
        {
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
