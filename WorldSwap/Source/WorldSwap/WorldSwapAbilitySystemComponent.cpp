// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldSwapAbilitySystemComponent.h"

float UWorldSwapAbilitySystemComponent::GetCooldownRemainingForTag(FGameplayTag CooldownTag) const
{
    FGameplayTagContainer Tags;
    Tags.AddTag(CooldownTag);

    const FGameplayEffectQuery Query =
        FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(Tags);

    const TArray<float> Times =
        GetActiveEffectsTimeRemaining(Query);

    float RemainingTime = 0.f;

    for (float Time : Times)
    {
        RemainingTime = FMath::Max(RemainingTime, Time);
    }

    return RemainingTime;
}

bool UWorldSwapAbilitySystemComponent::IsCooldownActive(FGameplayTag CooldownTag) const
{
    return GetCooldownRemainingForTag(CooldownTag) > 0.f;
}
