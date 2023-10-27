#include "EventSystem.h"

void EventSystem::RegisterEvent(const std::string& eventName, const EventCallback& callback)
{
    // add the callbac to a list of callbacks with the eventname
    eventCallbacks[eventName].push_back(callback);
}

// this function gives me so many problems it aint even FUNNY
void EventSystem::UnregisterEvent(const std::string& eventName, const EventCallback& callback)
{
    // find the eventName in the map
    auto it = eventCallbacks.find(eventName);
    if (it != eventCallbacks.end())
    {
        // get the list of callbacks associated with the eventName
        auto& callbacks = it->second;
        for (auto it = callbacks.begin(); it != callbacks.end();)
        {
            // see if the current callback matches the one to be unregistered
            if (AreCallbacksEqual(*it, callback))
            {
                // erase it
                it = callbacks.erase(it);
            }
            else
            {
                // move to the next callback
                ++it;
            }
        }
    }
}

void EventSystem::BroadcastEvent(const std::string& eventName)
{
    // find the eventname in the list
    auto it = eventCallbacks.find(eventName);
    if (it != eventCallbacks.end())
    {
        for (const EventCallback& callback : it->second)
        {
            // execute callback
            callback();
        }
    }
}

bool EventSystem::AreCallbacksEqual(const EventCallback& callback1, const EventCallback& callback2)
{
    // compare types bc std::function hates me
    return callback1.target_type() == callback2.target_type();
}
