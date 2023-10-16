#pragma once

#include <vector>
#include <functional>
#include "Event.h"

class EventManager {
public:
    static EventManager& GetInstance();

    void RegisterEvent(const std::function<void(Event&)>& callback, Event& event);
    void TriggerEvent(Event& event);

private:
    std::vector<std::pair<std::function<void(Event&)>, Event&>> eventCallbacks;
};