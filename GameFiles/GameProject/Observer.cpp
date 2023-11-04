//------------------------------------------------------------------------------
//
// File Name:	Observer.cpp
// Author(s):	TayLee Young
// Purpose:		Allows for objects to observe an event and react to it
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
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