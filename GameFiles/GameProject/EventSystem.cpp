#include "EventSystem.h"

void EventSystem::RegisterEvent(const std::string& eventName, const EventCallback& callback)
{
    eventCallbacks[eventName].push_back(callback);
}

// this function gives me so many problems it aint even FUNNY
void EventSystem::UnregisterEvent(const std::string& eventName, const EventCallback& callback)
{
    auto it = eventCallbacks.find(eventName);
    if (it != eventCallbacks.end())
    {
        auto& callbacks = it->second;
        for (auto it = callbacks.begin(); it != callbacks.end();)
        {
            if (AreCallbacksEqual(*it, callback))
            {
                it = callbacks.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

void EventSystem::BroadcastEvent(const std::string& eventName)
{
    auto it = eventCallbacks.find(eventName);
    if (it != eventCallbacks.end())
    {
        for (const EventCallback& callback : it->second)
        {
            callback();
        }
    }
}

bool EventSystem::AreCallbacksEqual(const EventCallback& callback1, const EventCallback& callback2)
{
    return callback1.target_type() == callback2.target_type();
}
