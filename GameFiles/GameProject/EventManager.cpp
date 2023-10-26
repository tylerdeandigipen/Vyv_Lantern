#include "EventManager.h"

// Used to register listener
// Event type is which event the listener is gonna look for
// and then listener will be executed when a specified event occurs
void EventManager::AddListener(int eventType, std::function<void(const Event&)> listener)
{
    // stores and associates even types with their listener
    eventListeners.push_back({ eventType, listener });
}

// Removes a listener
// Event Type is which event a listener should be removed from
// Which listener to remove
void EventManager::RemoveListener(int eventType, std::function<void(const Event&)> listener)
{
    //Loops through the list of listeners
    for (auto it = eventListeners.begin(); it != eventListeners.end(); ++it)
    {
        // checks if the event and listener match the ones passed in
        if (it->first == eventType && it->second.target<void(const Event&)>() == listener.target<void(const Event&)>())
        {
            // gets rid of the listener if found
            eventListeners.erase(it);
            return; // Found and removed the listener, exit the loop
        }
    }
}

// Triggers event listeners when a specific event occurs
// Event is which event to dispatch
void EventManager::DispatchEvent(const Event& event)
{
    // loops through all registered event listeners
    for (const auto& listener : eventListeners)
    {
        // for every listener, if theyre looking for that event...
        if (listener.first == event.GetType())
        {
            // passes in the event, allowing them to respond
            listener.second(event);
        }
    }
}

// this was a major pain in my ass to do btw