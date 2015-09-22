// Fill out your copyright notice in the Description page of Project Settings.
//#include "EngineUtils.h"
#include "GaslightGame.h"
#include "TargetComponent.h"
#include "Engine.h"
#define SCREENMSG(text) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, text) )
#define SCREENMSGF(fparam) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, FString::SanitizeFloat(fparam)) )
#define SCREENWAR(text) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, text) )
#define SCREENERR(text) (GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, text) )

// Sets default values for this component's properties
UTargetComponent::UTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	RegisterComponent();

	isTargetted = false;
	isTargetting = false;
	cooldown = 30;
}


// Called when the game starts
void UTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	isTargetted = false;

	//Adding a tag to every component with
	GetOwner()->Tags.Add(FName(TEXT("Targetable")));
	// ...
	
}


// Called every frame
void UTargetComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	// ...
	if (isTargetting)
	{
		if (cooldown <= 0)
		{
			//print("selecting target");
			SelectValidTarget();
			cooldown = 30;
		}
		else
			--cooldown;
	}
}

TArray<AActor*> UTargetComponent::GetTargets()
{
	TArray<AActor*> ActorList;

	for (TObjectIterator<UTargetComponent> Itr; Itr; ++Itr)
	{
		if (!(Itr->GetOwner()->GetName().Equals(GetOwner()->GetName())))
		{
			//SCREENMSG(Itr->GetOwner()->GetName());
			ActorList.Add(Itr->GetOwner());
		}
	}

	return ActorList;
}

TArray<AActor*> UTargetComponent::FilterValidTargets(TArray<AActor*> list, std::function<bool(const AActor*, const AActor*)> filter)
{
	AActor* myOwner = GetOwner();
	TArray<AActor*> filteredList;

	for (auto iter(list.CreateIterator()); iter; ++iter)
	{
		if (filter(myOwner, *iter))
		{
			filteredList.Add(*iter);
		}
	}

	return filteredList;
}

bool UTargetComponent::SelectValidTarget()
{
	auto filter = [](const AActor*me, const AActor*other)
	{
		//SCREENMSGF(FVector::DistSquared(me->GetActorLocation(), target->GetActorLocation()));
		UE_LOG(TargetingLog, Warning, TEXT("Me: %s, Target: %s||Distance: %f"), *me->GetName(), *other->GetName(), FVector::DistSquared(me->GetActorLocation(), other->GetActorLocation()))
		if (FVector::DistSquared(me->GetActorLocation(), other->GetActorLocation()) < 1000000.0f)
		{
			FVector myFacing = me->GetActorForwardVector();
			FVector targetVec = other->GetActorLocation() - me->GetActorLocation();

				//cos of angle between me and target
			float costheta = FVector::DotProduct(myFacing, targetVec) / (myFacing.Size() * targetVec.Size());

			if (costheta < 0.707107f) //Within 45 degrees of us
			{
				UE_LOG(TargetingLog, Warning, TEXT("Found a valid Target"))
				return true;
			}
		}
		return false;
	};

	TArray<AActor*> validTargetList = FilterValidTargets(GetTargets(), filter);
	if (validTargetList.Num() == 0)
	{

		return false;

	}
	//Cast<UTargetComponent>(target)->UnsetAsTarget();
	if (target)
	{
		TArray<UActorComponent*> components;
		target->GetComponents(components);
		for (int i = 0; i < components.Num(); ++i)
		{
			UTargetComponent* targetComp = Cast<UTargetComponent>(components[i]);
			if (targetComp)
			{
				targetComp->UnsetAsTarget();
			}
		}
	}
	

	FVector myLocation = GetOwner()->GetActorLocation();
	float distsquare;
	if (target)
	{
		distsquare = FVector::DistSquared(target->GetActorLocation(), myLocation);
	}
	else
	{
		distsquare = 10000000;
	}

	for (auto iter(validTargetList.CreateIterator()); iter; ++iter)
	{
		float newDS = FVector::DistSquared((*iter)->GetActorLocation(), myLocation);
		if (newDS < distsquare)
		{
			distsquare = newDS;
			target = (*iter);
		}
	}

	//Cast<UTargetComponent>(target)->SetAsTarget();
	{
		TArray<UActorComponent*> components;
		target->GetComponents(components);
		for (int i = 0; i < components.Num(); ++i)
		{
			UTargetComponent* targetComp = Cast<UTargetComponent>(components[i]);
			if (targetComp)
			{
				targetComp->SetAsTarget();
			}
		}
	}
	return true;
}

void UTargetComponent::SetAsTarget()
{
	SCREENMSG("Target Locked!");
	isTargetted = true;
}

void UTargetComponent::UnsetAsTarget()
{
	isTargetted = false;
}


FVector UTargetComponent::GetDirectionToTarget()
{
	if (target)
	{
		FVector ourLoc = GetOwner()->GetActorLocation();
		FVector targetLoc = target->GetActorLocation();

		return targetLoc - ourLoc;
	}
	else
		return FVector(); //Default to zero vector (i hope)
}