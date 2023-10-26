#pragma once

#include <vector>
#include <functional>
#include "Event.h"

class EventManager
{
public:
    void AddListener(int eventType, std::function<void(const Event&)> listener);
    void RemoveListener(int eventType, std::function<void(const Event&)> listener);
    void DispatchEvent(const Event& event);

private:
    std::vector<std::pair<int, std::function<void(const Event&)>>> eventListeners;
};
