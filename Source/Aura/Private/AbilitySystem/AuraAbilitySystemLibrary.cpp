// Copyright PHunter Studios


#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Game/AuraGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "UI/WidgetController/AuraWidgetController.h"
#include "UI/HUD/AuraHUD.h"
#include "Player/AuraPlayerState.h"

UOverlayWidgetController *UAuraAbilitySystemLibrary::GetOverlayWidgetController(const UObject *WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
        {
            AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
            UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
            UAttributeSet* AS = PS->GetAttributeSet();
            const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
            return AuraHUD->GetOverlayWidgetController(WidgetControllerParams);
        }
    }
    return nullptr;
}

UAttributeMenuWidgetController *UAuraAbilitySystemLibrary::GetAttributeMenuWidgetController(const UObject *WorldContextObject)
{
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
    {
        if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(PC->GetHUD()))
        {
            AAuraPlayerState* PS = PC->GetPlayerState<AAuraPlayerState>();
            UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
            UAttributeSet* AS = PS->GetAttributeSet();
            const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
            return AuraHUD->GetAttributeMenuWidgetController(WidgetControllerParams);
        }
    }
    return nullptr;
}

void UAuraAbilitySystemLibrary::InitialiseDefaultAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
    AActor* AvatarActor = ASC->GetAvatarActor();
    UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
    checkf(CharacterClassInfo, TEXT("CharacterClassInfo is null (DA_CharacterClassInfo not found / not set)."));

    const FCharacterClassDefaultInfo ClassDefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

    checkf(ClassDefaultInfo.PrimaryAttributes,
        TEXT("PrimaryAttributes GE is null for %s"), *UEnum::GetValueAsString(CharacterClass));

    checkf(CharacterClassInfo->SecondaryAttributes,
        TEXT("SecondaryAttributes GE is null (Common Class Defaults)"));

    checkf(CharacterClassInfo->VitalAttributes,
        TEXT("VitalAttributes GE is null (Common Class Defaults)"));


    FGameplayEffectContextHandle PrimaryAttributesContextHandle = ASC->MakeEffectContext();
    PrimaryAttributesContextHandle.AddSourceObject(AvatarActor);

    const FGameplayEffectSpecHandle PrimaryAttributesSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributesContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributesSpecHandle.Data.Get());

    FGameplayEffectContextHandle SecondaryAttributesContextHandle = ASC->MakeEffectContext();
    SecondaryAttributesContextHandle.AddSourceObject(AvatarActor);

    const FGameplayEffectSpecHandle SecondaryAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->SecondaryAttributes, Level, SecondaryAttributesContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributesSpecHandle.Data.Get());

    FGameplayEffectContextHandle VitalAttributesContextHandle = ASC->MakeEffectContext();
    VitalAttributesContextHandle.AddSourceObject(AvatarActor);

    const FGameplayEffectSpecHandle VitalAttributesSpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->VitalAttributes, Level, VitalAttributesContextHandle);
    ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributesSpecHandle.Data.Get());

}

void UAuraAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{

    UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);

    // Give common abilities
    for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
    {
        FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
        ASC->GiveAbility(AbilitySpec);
    }
}

UCharacterClassInfo* UAuraAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
    AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
    if (AuraGameMode == nullptr) return nullptr;

    return AuraGameMode->CharacterClassInfo;
}