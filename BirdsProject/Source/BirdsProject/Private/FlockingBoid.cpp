#include "FlockingBoid.h"
#include "Kismet/KismetMathLibrary.h"

UFlockingBoid::UFlockingBoid()
{
    PrimaryComponentTick.bCanEverTick = false;
    _isFlocking = false;
}

void UFlockingBoid::StartFlocking(AActor* leader, float rowDistance, float rowSpacing, float verticalSpacing, TArray<AActor*> flockMembers, float flockDelay, float flockOffset)
{
    _leader = leader;
    _rowDistance = rowDistance;
    _rowSpacing = rowSpacing;
    _verticalSpacing = verticalSpacing;
    _flockMembers = flockMembers;
    _flockDelay = flockDelay;
    _flockOffset = flockOffset;
    _skeletalMeshComponent = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
    _movementComponent = GetOwner()->FindComponentByClass<UFloatingPawnMovement>();
    _isFlocking = true;
}


void UFlockingBoid::Flock(float DeltaTime)
{
    if (_isFlocking)
    {
        FVector MoveDirection = GetOwner()->GetActorForwardVector();
        _movementComponent->AddInputVector(MoveDirection * _movementComponent->MaxSpeed);

        if (GetOwner() == _leader)
        {
                FRotator CurrentRotation = GetOwner()->GetActorRotation();
                FRotator RandomRotation = FRotator(FMath::FRandRange(-90.0f, 90.0f), FMath::FRandRange(-90.0f, 90.0f), FMath::FRandRange(-90.0f, 90.0f));
                FRotator NewRotation = FMath::RInterpTo(CurrentRotation, CurrentRotation + RandomRotation, DeltaTime, 2.0f);
                GetOwner()->SetActorRotation(NewRotation);
        }

        else if (_leader)
        {
            FVector directionToLeader = _leader->GetActorLocation() - GetOwner()->GetActorLocation();
           // directionToLeader.Normalize();

            FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + directionToLeader);
            FRotator CurrentRotation = GetOwner()->GetActorRotation();
            FRotator NewRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, DeltaTime, 2.0f);
            GetOwner()->SetActorRotation(NewRotation);

            FVector BirdPosition = GetOwner()->GetActorLocation();
            BirdPosition += GetOwner()->GetActorForwardVector() * _rowDistance;
            BirdPosition += GetOwner()->GetActorRightVector() * _rowSpacing;
            BirdPosition += GetOwner()->GetActorUpVector() * _verticalSpacing;

            int32 Index = _flockMembers.Find(GetOwner());
            BirdPosition += GetOwner()->GetActorRightVector() * (Index * _flockOffset);

            FVector NewLocation = FMath::VInterpTo(GetOwner()->GetActorLocation(), BirdPosition, DeltaTime, _flockDelay);
            GetOwner()->SetActorLocation(NewLocation);
        }
    }
}