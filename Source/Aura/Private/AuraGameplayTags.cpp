// Copyright PHunter Studios


#include "AuraGameplayTags.h"
#include "GameplayTagsManager.h"

FAuraGameplayTags FAuraGameplayTags::GameplayTags;

void FAuraGameplayTags::InitializeNativeTags()
{
    /**
     * Primary Attributes
    */
    GameplayTags.Attributes_Primary_Strength = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Strength"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Primary_Intelligence = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Intelligence"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Primary_Resilience = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Resilience"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Primary_Vitality = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Primary.Vitality"), FString("Reduces damage taken"));

    /**
     * Secondary Attributes
    */
    GameplayTags.Attributes_Secondary_Armour = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armour"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_ArmourPenetration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ArmourPenetration"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_BlockChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.BlockChance"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_MagicResist = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MagicResist"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_PhysicalDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.PhysicalDamage"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_MagicDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MagicDamage"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_PhysicalResist = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.PhysicalResist"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_CriticalHitChance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitChance"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_CriticalHitDamage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitDamage"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_CriticalHitResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.CriticalHitResistance"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_Stamina = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Stamina"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_ManaRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.ManaRegeneration"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_HealthRegeneration = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.HealthRegeneration"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_StatusResistance = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.StatusResistance"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_MaxMana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxMana"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Secondary_MaxHealth = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.MaxHealth"), FString("Reduces damage taken"));

    /**
     * Vital Attributes
    */
    GameplayTags.Attributes_Vital_Health = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Health"), FString("Reduces damage taken"));
    GameplayTags.Attributes_Vital_Mana = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Vital.Mana"), FString("Reduces damage taken"));

    /**
     * Input Tags
    */
    GameplayTags.InputTag_LMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.LMB"), FString("Input Tag for Left Mouse Button"));
    GameplayTags.InputTag_RMB = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.RMB"), FString("Input Tag for Right Mouse Button"));
    GameplayTags.InputTag_1 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.1"), FString("Input Tag for Key 1"));
    GameplayTags.InputTag_2 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.2"), FString("Input Tag for Key 2"));
    GameplayTags.InputTag_3 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.3"), FString("Input Tag for Key 3"));
    GameplayTags.InputTag_4 = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("InputTag.4"), FString("Input Tag for Key 4"));

    GameplayTags.Damage = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage"), FString("Damage"));

    /**
     * Damage Types
     */
    GameplayTags.Damage_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Fire"), FString("Fire Damage Type"));
    GameplayTags.Damage_Magical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Magical"), FString("Magical Damage Type"));
    GameplayTags.Damage_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Lightning"), FString("Lightning Damage Type"));
    GameplayTags.Damage_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Damage.Physical"), FString("Physical Damage Type"));

    /**
     * Resistances
     */
    GameplayTags.Attributes_Resistance_Fire = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Fire"), FString("Fire Resistance"));
    GameplayTags.Attributes_Resistance_Lightning = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Lightning"), FString("Lightning Resistance"));
    GameplayTags.Attributes_Resistance_Physical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Physical"), FString("Physical Resistance"));
    GameplayTags.Attributes_Resistance_Magical = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Resistance.Magical"), FString("Magical Resistance"));

    /**
     * Map of Damage Types to Resistances
     */
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Fire, GameplayTags.Attributes_Resistance_Fire);
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Lightning, GameplayTags.Attributes_Resistance_Lightning);
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Physical, GameplayTags.Attributes_Resistance_Physical);
    GameplayTags.DamageTypesToResistances.Add(GameplayTags.Damage_Magical, GameplayTags.Attributes_Resistance_Magical);

    /**
     * Effects
     */
    GameplayTags.Effects_HitReact = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Effects.HitReact"), FString("Reaction to being hit"));

}
