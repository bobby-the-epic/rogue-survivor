#include "EventBus.h"

UEventBus* UEventBus::Instance = nullptr;

UEventBus* UEventBus::Get()
{
    if (!Instance)
    {
        Instance = NewObject<UEventBus>();
        Instance->AddToRoot();
    }
    return Instance;
}
