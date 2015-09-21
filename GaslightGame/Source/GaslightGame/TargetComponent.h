// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include <functional> //lambda function shenanigans

#include "EngineUtils.h"
#include "Components/ActorComponent.h"
#include "TargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASLIGHTGAME_API UTargetComponent : public UActorComponent
{
	GENERATED_BODY()

		
public:
	UPROPERTY(VisibleAnywhere)
	AActor* target;
	
	UPROPERTY(VisibleAnywhere)
	bool isTargetted;	//Is this targetted by something?
	
	UPROPERTY(EditAnywhere)
	bool isTargetting;	//Is this person targetting?

	UPROPERTY(VisibleAnywhere)
	uint8 cooldown;	//Just so we aren't checking valid targets every frame (expensive!)
	// Sets default values for this component's properties
	UTargetComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	
	TArray<AActor*> GetTargets();								//Get all possibly targets
	TArray<AActor*> FilterValidTargets(TArray<AActor *> list, std::function<bool (const AActor*,const AActor *)> filter);	//Filter target list
	bool SelectValidTarget();									//Pick the target
	
	void SetAsTarget();						//Helperfunctions for others
	void UnsetAsTarget();
	//Returns vector to the target
	FVector GetDirectionToTarget();

};
