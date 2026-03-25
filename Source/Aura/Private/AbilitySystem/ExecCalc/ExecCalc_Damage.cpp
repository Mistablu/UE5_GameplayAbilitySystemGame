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

    DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(MagicalResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance);
    DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance);

    TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefs;
    AuraDamageStatics()
    {
        //Capture attribute from source or target, with or without snapshotting
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, Armour, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, ArmourPenetration, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, BlockChance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitChance, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitDamage, Source, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, CriticalHitResistance, Target, false);

        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, PhysicalResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, MagicalResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, FireResistance, Target, false);
        DEFINE_ATTRIBUTE_CAPTUREDEF(UAuraAttributeSet, LightningResistance, Target, false);

        TagsToCaptureDefs.Add(FAuraGameplayTags::Get().Attributes_Secondary_Armour, ArmourDef);
        TagsToCaptureDefs.Add(FAuraGameplayTags::Get().Attributes_Secondary_ArmourPenetration, ArmourPenetrationDef);
        TagsToCaptureDefs.Add(FAuraGameplayTags::Get().Attributes_Secondary_BlockChance, BlockChanceDef);
        TagsToCaptureDefs.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitChance, CriticalHitChanceDef);
        TagsToCaptureDefs.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitDamage, CriticalHitDamageDef);
        TagsToCaptureDefs.Add(FAuraGameplayTags::Get().Attributes_Secondary_CriticalHitResistance, CriticalHitResistanceDef);

        TagsToCaptureDefs.Add(FAuraGameplayTags::Get().Attributes_Resistance_Physical, PhysicalResistanceDef);
        TagsToCaptureDefs.Add(FAuraGameplayTags::Get().Attributes_Resistance_Magical, MagicalResistanceDef);
        TagsToCaptureDefs.Add(FAuraGameplayTags::Get().Attributes_Resistance_Fire, FireResistanceDef);
        TagsToCaptureDefs.Add(FAuraGameplayTags::Get().Attributes_Resistance_Lightning, LightningResistanceDef);
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

    RelevantAttributesToCapture.Add(DamageStatics().PhysicalResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().MagicalResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().FireResistanceDef);
    RelevantAttributesToCapture.Add(DamageStatics().LightningResistanceDef);
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
    float Damage = 0;
    for (const TTuple<FGameplayTag, FGameplayTag>& Pair : FAuraGameplayTags::Get().DamageTypesToResistances)
    { 
        const FGameplayTag DamageTypeTag = Pair.Key;
        const FGameplayTag ResistanceTag = Pair.Value;

        checkf(AuraDamageStatics().TagsToCaptureDefs.Contains(ResistanceTag), TEXT("TagsToCaptureDefs does not contain Tag: [%s] in ExecCalc_Damage."), *ResistanceTag.ToString())
        const FGameplayEffectAttributeCaptureDefinition CaptureDef = AuraDamageStatics().TagsToCaptureDefs[ResistanceTag];

        float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);

        float Resistance = 0.f;
        ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(CaptureDef, EvaluationParameters, Resistance);
        Resistance = FMath::Clamp(Resistance, 0.f, 100.f);

        DamageTypeValue *= (100 - Resistance) / 100;
        Damage += DamageTypeValue;
    }

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