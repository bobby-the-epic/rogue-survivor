#include "EventBus.h"

UEventBus* UEventBus::Instance = nullptr;

UEventBus::UEventBus()
{
    Instance = this;
}
UEventBus* UEventBus::Get()
{
    return Instance;
}
