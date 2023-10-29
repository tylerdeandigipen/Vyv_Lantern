#include "Observer.h"
#include "Logging.h"

Logging& loggerObs = Logging::GetInstance("debugLog.log");

void PlayerMoveLeftObserver::onNotify(const Entity& entity, EventType event) 
{
    switch (event)
    {
    case EventType::PLAYER_MOVED_LEFT:
        loggerObs.LogLine("event working");
        break;
    default:
        break;
    }
}