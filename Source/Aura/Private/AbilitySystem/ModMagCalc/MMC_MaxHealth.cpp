// Copyright PHunter Studios


#include "AbilitySystem/ModMagCalc/MMC_MaxHealth.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"

UMMC_MaxHealth::UMMC_MaxHealth()
{
    VitalityDef.AttributeToCapture = UAuraAttributeSet::GetVitalityAttribute();
    VitalityDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
    VitalityDef.bSnapshot = false;

    RelevantAttributesToCapture.Add(VitalityDef);
}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec &Spec) const
{
    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;
    
    float Vitality = 0;
    GetCapturedAttributeMagnitude(VitalityDef, Spec, EvaluationParameters, Vitality);
    Vitality = FMath::Max<float>(Vitality, 0);

    ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());
    const int32 PlayerLevel = CombatInterface->GetPlayerLevel();

    return 100+5*Vitality+10*PlayerLevel;
}