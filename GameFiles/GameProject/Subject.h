//------------------------------------------------------------------------------
//
// File Name:	Subject.h
// Author(s):	TayLee Young
// Purpose:		Subject class declaration 
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once
#include "Observer.h"

class Subject 
{
private:
    Observer* head_;

public:
    Subject() : head_(nullptr) {}

    void addObserver(Observer* observer);

    void removeObserver(Observer* observer);

    void notify(const Entity& entity, EventType event);
};

