#pragma once

enum class EventType {
    // put events here
};

class Event {
public:
    Event(EventType type) : type_(type) {}

    EventType getType() const {
        return type_;
    }

private:
    EventType type_;
};
