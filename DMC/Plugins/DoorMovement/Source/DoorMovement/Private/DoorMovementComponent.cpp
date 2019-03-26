// Copyright 2019 Inu Games

#include "DoorMovementComponent.h"


UDoorMovementComponent::UDoorMovementComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Rotating components are often added to actors also with a projectile component,
	// and we wouldn't want to stomp their desired physics volume setting so turn off auto-registration.
	bAutoRegisterPhysicsVolumeUpdates = false;
	bComponentShouldUpdatePhysicsVolume = false;
	bAutoActivate = false;


	Position = 0.0f;
	Speed = 15.0f;
	OpenOffset = FVector(0.0f, 120.0f, 0.0f);
	//RotationRate.Yaw = 180.0f;
	//bRotationInLocalSpace = true;
}


void UDoorMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	// skip if we don't want component updated when not rendered or if updated component can't move
	if (ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!IsValid(UpdatedComponent))
	{
		return;
	}

	if (Position == NextPosition)
	{
		Deactivate();
		return;
	}

	const float OldPosition = Position;
	Position = FMath::FInterpTo(Position, NextPosition, DeltaTime, Speed);

	const FQuat OldRotation = UpdatedComponent->GetComponentQuat();

	const FVector Scale = UseScale ? UpdatedComponent->GetComponentScale() : FVector(1.0f, 1.0f, 1.0f);
	const FVector DeltaLocation = OldRotation.RotateVector((OpenOffset * Scale) * (Position-OldPosition));


	//if (!PivotTranslation.IsZero())
	//{
	//	//const FVector OldPivot = OldRotation.RotateVector(PivotTranslation);
	//	//const FVector NewPivot = NewRotation.RotateVector(PivotTranslation);
	//	//DeltaLocation = (OldPivot - NewPivot); // ConstrainDirectionToPlane() not necessary because it's done by MoveUpdatedComponent() below.
	//}

	const bool bEnableCollision = false;
	MoveUpdatedComponent(DeltaLocation, OldRotation, bEnableCollision);
}

void UDoorMovementComponent::SetPosition(float NewPosition)
{
	if (bIsActive)
	{
		return;
	}

	if (NewPosition != Position)
	{
		NextPosition = NewPosition;
		Activate();
	}
}

void UDoorMovementComponent::Toggle()
{
	if (bIsActive)
	{
		return;
	}

	if (Position == 0.0f)
	{
		NextPosition = 1.0f;
	}
	else
	if (Position == 1.0f)
	{
		NextPosition = 0.0f;
	}
	else
	
	{
		NextPosition = (Position > 0.5f) ? 1.0f : 0.0f;
	}

	Activate();
}

bool UDoorMovementComponent::AddPosition(float Increment)
{
	const float OldNextPosition = NextPosition;
	NextPosition = FMath::Clamp(NextPosition + Increment, 0.0f, 1.0f);

	if (!bIsActive && NextPosition != Position)
	{	
		Activate();
	}

	return OldNextPosition != NextPosition;
}

#if WITH_EDITOR
void UDoorMovementComponent::PreEditChange(UProperty* PropertyAboutToChange)
{
	if ((PropertyAboutToChange != nullptr) && (PropertyAboutToChange->GetFName() == GET_MEMBER_NAME_CHECKED(UDoorMovementComponent, Position)))
	{
		//record position which is about to change for use in posteditchange
		OldPositionEd = Position;
	}

	Super::PreEditChange(PropertyAboutToChange);
}
void UDoorMovementComponent::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	const FName PropertyName = (PropertyChangedEvent.Property ? PropertyChangedEvent.Property->GetFName() : NAME_None);


	if ((PropertyName == GET_MEMBER_NAME_CHECKED(UDoorMovementComponent, Position)))
	{
		
		if (AActor* MyActor = GetOwner())
		{
			if (USceneComponent* NewUpdatedComponent = MyActor->GetRootComponent())
			{

				const FQuat Rotation = NewUpdatedComponent->GetComponentQuat();
				const FVector Scale = UseScale ? NewUpdatedComponent->GetComponentScale() : FVector(1.0f, 1.0f, 1.0f);
				const FVector DeltaLocation = Rotation.RotateVector((OpenOffset * Scale) * (Position - OldPositionEd));

				const FVector NewDelta = ConstrainDirectionToPlane(DeltaLocation);

				const bool bEnableCollision = false;
				NewUpdatedComponent->MoveComponent(NewDelta, Rotation, bEnableCollision);
			}
		}

	}
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

void UDoorMovementComponent::BeginPlay()
{
	NextPosition = Position;	
	Super::BeginPlay();
}
