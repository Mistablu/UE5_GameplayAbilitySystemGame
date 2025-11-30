// Copyright PHunter Studios


#include "UI/WidgetController/AuraWidgetController.h"

void UAuraWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
    PlayerController = WCParams.PlayerController;
    PlayerState = WCParams.PlayerState;
    AbilitySystemComponent = WCParams.AbilitySystemComponent;
    AttributeSet = WCParams.AttributeSet;

}
void UAuraWidgetController::BroadcastInitialValues()
{
    // Intended to be overridden in subclasses
}
void UAuraWidgetController::BindCallbacksToDependencies()
{
    // Intended to be overridden in subclasses
}