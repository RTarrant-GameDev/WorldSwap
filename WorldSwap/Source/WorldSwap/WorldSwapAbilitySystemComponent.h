// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "AbilitySystemComponent.h"
#include "WorldSwapAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class WORLDSWAP_API UWorldSwapAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:

    UFUNCTION(BlueprintCallable, Category = "Cooldowns")
    float GetCooldownRemainingForTag(
        FGameplayTag CooldownTag) const;

    UFUNCTION(BlueprintCallable, Category = "Cooldowns")
    bool IsCooldownActive(
        FGameplayTag CooldownTag) const;
};
