#include "FlockingBoid.h"
#include "Kismet/KismetMathLibrary.h"

UFlockingBoid::UFlockingBoid()
{
    PrimaryComponentTick.bCanEverTick = false;
    _isFlocking = false;
}

void UFlockingBoid::StartFlocking(AActor* leader, float rowDistance, float rowSpacing, float verticalSpacing, TArray<AActor*> flockMembers, float flockDelay, float flockOffset, USplineComponent* splineComponent)
{
    _leader = leader;
    _rowDistance = rowDistance;
    _rowSpacing = rowSpacing;
    _verticalSpacing = verticalSpacing;
    _flockMembers = flockMembers;
    _flockDelay = flockDelay;
    _flockOffset = flockOffset;
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

        //if (GetOwner() == _leader)
        //{
        //    FVector splineLocation = _splineComponent->GetLocationAtTime(splineInterpTime, ESplineCoordinateSpace::World);
        //    FVector splineTangent = _splineComponent->GetTangentAtTime(splineInterpTime, ESplineCoordinateSpace::World);
        //    FVector randomOffset = FVector(FMath::FRandRange(-randomRange, randomRange), FMath::FRandRange(-randomRange, randomRange), FMath::FRandRange(-randomRange, randomRange));
        //    splineLocation += randomOffset;
        //    GetOwner()->SetActorLocation(splineLocation);
        //    FRotator DesiredRotation = splineTangent.Rotation();
        //    GetOwner()->SetActorRotation(DesiredRotation);

        //    splineInterpTime += deltaTime * splineInterpSpeed;
        //    splineInterpTime = FMath::Fmod(splineInterpTime, 1.0f);
        //}
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
            GetOwner()->SetActorRotation(NewRotation.Rotator());

            FVector BirdPosition = GetOwner()->GetActorLocation();
            BirdPosition += GetOwner()->GetActorForwardVector() * _rowDistance;
            BirdPosition += GetOwner()->GetActorRightVector() * _rowSpacing;
            BirdPosition += GetOwner()->GetActorUpVector() * _verticalSpacing;

            int32 Index = _flockMembers.Find(GetOwner());
            BirdPosition += GetOwner()->GetActorRightVector() * (Index * _flockOffset);

            FVector NewLocation = FMath::VInterpTo(GetOwner()->GetActorLocation(), BirdPosition, deltaTime, _flockDelay);
            GetOwner()->SetActorLocation(NewLocation);
        }
    }
}
