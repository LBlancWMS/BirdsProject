#include "FlockingBoid.h"
#include "Kismet/KismetMathLibrary.h"

UFlockingBoid::UFlockingBoid()
{
    PrimaryComponentTick.bCanEverTick = false;
    _isFlocking = false;
}

void UFlockingBoid::StartFlocking(AActor* leader, float rowDistance, float rowSpacing, float verticalSpacing, TArray<AActor*> flockMembers)
{
    _leader = leader;
    _rowDistance = rowDistance;
    _rowSpacing = rowSpacing;
    _verticalSpacing = verticalSpacing;
    _flockMembers = flockMembers;
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
            FRotator CurrentRotation = _skeletalMeshComponent->GetComponentRotation();
            FRotator RandomRotation = FRotator(FMath::FRandRange(-90.0f, 90.0f), FMath::FRandRange(-90.0f, 90.0f), 0.0f);
            FRotator NewRotation = FMath::RInterpTo(CurrentRotation, CurrentRotation + RandomRotation, DeltaTime, 2.0f);
            _skeletalMeshComponent->SetWorldRotation(NewRotation);
        }

        else if (_leader)
        {
            FVector directionToLeader = _leader->GetActorLocation() - GetOwner()->GetActorLocation();
            directionToLeader.Normalize();

            FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetOwner()->GetActorLocation(), GetOwner()->GetActorLocation() + directionToLeader);
            FRotator CurrentRotation = _skeletalMeshComponent->GetComponentRotation();
            FRotator NewRotation = FMath::RInterpTo(CurrentRotation, LookAtRotation, DeltaTime, 2.0f);
            _skeletalMeshComponent->SetWorldRotation(NewRotation);

            FVector RelativePosition = GetOwner()->GetActorLocation();
            RelativePosition += GetOwner()->GetActorForwardVector() * _rowDistance;
            RelativePosition += GetOwner()->GetActorRightVector() * _rowSpacing;
            RelativePosition += GetOwner()->GetActorUpVector() * _verticalSpacing;

            int32 Index = _flockMembers.Find(GetOwner());
            RelativePosition += GetOwner()->GetActorRightVector() * (Index * 50.0f);

            FVector NewLocation = FMath::VInterpTo(GetOwner()->GetActorLocation(), RelativePosition, DeltaTime, 5.0f);
            GetOwner()->SetActorLocation(NewLocation);
        }
    }
}