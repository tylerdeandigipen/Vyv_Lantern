#pragma once

#include "EventBitmap.h"

class Event
{
public:
    Event(int eventType, int entityId) : eventType(eventType), entityId(entityId) {}

    int GetType() const;
    int GetEntityId() const;

private:
    int eventType;
    int entityId;
};
