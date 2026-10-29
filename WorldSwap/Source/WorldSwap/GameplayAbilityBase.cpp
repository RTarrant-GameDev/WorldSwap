// Fill out your copyright notice in the Description page of Project Settings.


#include "GameplayAbilityBase.h"
#include "WorldSwapCharacter.h"

UGameplayAbilityBase::UGameplayAbilityBase()
{
	// keep this; even if instances are per activation, we no longer store state here
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

bool UGameplayAbilityBase::CommitAbility(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo,
	FGameplayTagContainer* OptionalRelevantTags)
{
	const bool bCommitted =
		Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);

	return bCommitted;
}