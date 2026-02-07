// Copyright PHunter Studios

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AuraGameplayTags.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"
#include "AuraAbilityTypes.h"

struct AuraDamageStatics
{
    DECLARE_ATTRIBUTE_CAPTUREDEF(Armour);
    DECLARE_ATTRIBUTE_CAPTUREDEF(ArmourPenetration);
    DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
    DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
    AuraDamageStatics()
    {
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armour, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmourPenetration, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);
    }
};

static const AuraDamageStatics& DamageStatics()
{
    static AuraDamageStatics DStatics;
    return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
    RelevantAttributesToCapture.Add(DamageStatics().ArmourDef);
    RelevantAttributesToCapture.Add(DamageStatics().ArmourPenetrationDef);
    RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
    RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
    const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
    const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

    AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
    AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
    ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
    ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

    const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

    const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
    const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags(); 
    FAggregatorEvaluateParameters EvaluationParameters;
    EvaluationParameters.SourceTags = SourceTags;
    EvaluationParameters.TargetTags = TargetTags;

    //Get Damage Set by Caller Magnitude
    float Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage);
    float TargetBlockChance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
    TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0);

    //Halve damage on block
    const bool bBlocked = FMath::RandRange(1, 100) < TargetBlockChance;

    FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
    UAuraAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocked);

    Damage = bBlocked ? Damage / 2 : Damage;

    //Determine Armour
    float TargetArmour = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmourDef, EvaluationParameters, TargetArmour);
    TargetArmour = FMath::Max<float>(TargetArmour, 0);

    //Determine Armour Penetration
    float SourceArmourPenetration = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmourPenetrationDef, EvaluationParameters, SourceArmourPenetration);
    SourceArmourPenetration = FMath::Max<float>(SourceArmourPenetration, 0);

    //Determine Critical Hit Chance/Damage/Resistance
    float SourceCriticalHitChance = 0.f;
    float SourceCriticalHitDamage = 0.f;
    float TargetCriticalHitResistance = 0.f;
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalHitChance);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalHitDamage);
    ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalHitResistance);
    SourceCriticalHitChance = FMath::Max<float>(SourceCriticalHitChance, 0);
    SourceCriticalHitDamage = FMath::Max<float>(SourceCriticalHitDamage, 0);
    TargetCriticalHitResistance = FMath::Max<float>(TargetCriticalHitResistance, 0);

    const UCharacterClassInfo* CharacterClassInfo = UAuraAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
    const FRealCurve* ArmourPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmourPenetration"), FString());
    const float ArmourPenetrationCoefficient = ArmourPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());

    const float EffectiveArmour = TargetArmour * (100 - SourceArmourPenetration * ArmourPenetrationCoefficient) / 100;

    const FRealCurve* EffectiveArmourCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmour"), FString());
    const float EffectiveArmourCoefficient = EffectiveArmourCurve->Eval(TargetCombatInterface->GetPlayerLevel());
    Damage *= (100 - EffectiveArmour * EffectiveArmourCoefficient) / 100;

    const bool bCriticalHit = FMath::RandRange(1, 100) < (SourceCriticalHitChance - TargetCriticalHitResistance);
    UAuraAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCriticalHit);
    Damage = bCriticalHit ? Damage * (1 + SourceCriticalHitDamage / 100) : Damage;

    const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
    OutExecutionOutput.AddOutputModifier(EvaluatedData);
    

}