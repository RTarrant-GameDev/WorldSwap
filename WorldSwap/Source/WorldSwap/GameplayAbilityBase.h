// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class WORLDSWAP_API UGameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGameplayAbilityBase();

protected:
	UPROPERTY(EditAnywhere, Category = "Ability Name")
	FString AbilityName;

protected:
	// increment uses when it successfully commits
	virtual bool CommitAbility(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo,
		FGameplayTagContainer* OptionalRelevantTags = nullptr) override;
};
