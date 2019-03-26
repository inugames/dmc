// Copyright 2019 Inu Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/MovementComponent.h"
#include "DoorMovementComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = Movement, meta = (BlueprintSpawnableComponent), HideCategories = (Velocity))
class DOORMOVEMENT_API UDoorMovementComponent : public UMovementComponent
{
	GENERATED_UCLASS_BODY()

	//Begin UActorComponent Interface
	/** Applies rotation to UpdatedComponent. */
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
	//End UActorComponent Interface
	
	//Changes door position, 0 is closed, 1 is open
	UFUNCTION(BlueprintCallable)
	void SetPosition(float NewPosition);

	//Open if closed, close if open, move to the nearest extremity othrwise
	UFUNCTION(BlueprintCallable)
	void Toggle();

	//Move a by an increment, clamped at 0-1 range
	UFUNCTION(BlueprintCallable)
	bool AddPosition(float Increment);

	//Position, 0 is closed, 1 is open. Clamped at 0-1 range.
	UPROPERTY(Category = "Door Movement", BlueprintReadWrite, EditAnywhere, meta = (UIMin = 0, ClampMin = 0, UIMax = 1, ClampMax = 1))
	float Position;

	//Speed of the door movement
	UPROPERTY(Category = "Door Movement", BlueprintReadWrite, EditAnywhere)
	float Speed;

	//Relative position of the fully open door
	UPROPERTY(Category = "Door Movement", BlueprintReadWrite, EditAnywhere)
	FVector OpenOffset;

	//Apply scale to the OpenOffset, othewise it will always be in world space 
	UPROPERTY(Category = "Door Movement", BlueprintReadWrite, EditAnywhere)
	bool UseScale;

#if WITH_EDITOR
	//~ Begin UObject Interface.
	virtual void PreEditChange(UProperty* PropertyAboutToChange) override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	//~ End UObject Interface.
	float OldPositionEd;
#endif // WITH_EDITOR

	virtual void BeginPlay() override;

	float NextPosition;

};
