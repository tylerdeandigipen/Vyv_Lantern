#pragma once

#include <functional>
#include <map>
#include <vector>
#include <unordered_map>
#include <string>

class EventSystem
{
public:
    using EventCallback = std::function<void()>;

    static EventSystem* GetInstance();

    void RegisterEvent(const std::string& eventName, const EventCallback& callback);
    void UnregisterEvent(const std::string& eventName, const EventCallback& callback);
    void BroadcastEvent(const std::string& eventName);

private:
    EventSystem() = default;
    EventSystem(const EventSystem&) = delete;
    EventSystem& operator=(const EventSystem&) = delete;

    std::unordered_map<std::string, std::vector<EventCallback>> eventCallbacks;
};