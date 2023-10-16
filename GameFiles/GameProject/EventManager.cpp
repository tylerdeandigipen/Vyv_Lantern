#include "EventManager.h"

EventManager& EventManager::GetInstance()
{
	static EventManager instance;
	return instance;
}

void EventManager::RegisterEvent(const std::function<void(Event&)>& callback, Event& event) 
{
	/* adds an elemnt (in this case, an event or function) to the end of the vector */
	eventCallbacks.emplace_back(callback, event);
}

void EventManager::TriggerEvent(Event& event)
{
	/* for each loop, callbackPair is each element, and eventCallbacks is the container
	   aka, for each pair in the callback container, do this */
	for (const std::pair<std::function<void(Event&)>, Event&> &callbackPair : eventCallbacks) 
	{
		if (&event == &(callbackPair.second)) 
		{
			callbackPair.first(event);
		}
	}
}