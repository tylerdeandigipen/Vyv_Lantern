//------------------------------------------------------------------------------
//
// File Name:	Observer.h
// Author(s):	TayLee Young
// Purpose:		Observer class declaration 
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Entity.h"
#include "Event.h"

class Subject; //fForward declaration

class Observer 
{
public:
    friend class Subject; // allow Subject to access the "next" pointer bc linked list

    Observer() : observerNext(nullptr) {}
    virtual ~Observer() {}

    virtual void onNotify(const Entity& entity, EventType event) = 0;

private:
    Observer* observerNext;
};

class PlayerMoveLeftObserver : public Observer 
{
public:
    virtual void onNotify(const Entity& entity, EventType eventType);
};