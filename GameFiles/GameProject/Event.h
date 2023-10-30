#pragma once

enum class EventType 
{
    PLAYER_MOVED_LEFT,
};

class Event 
{
public:
    Event(EventType type) : type_(type) {}

    EventType getType() const {
        return type_;
    }

private:
    EventType type_;
};
