//------------------------------------------------------------------------------
//
// File Name:	Event.h
// Author(s):	TayLee Young
// Purpose:		Declaration of EventType enum and Event class.
//
// Copyright © 2023 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------
#pragma once

enum class EventType
{
	PLAYER_MOVED_LEFT,
	PLAYER_MOVED_RIGHT,
	PLAYER_MOVED_UP,
	PLAYER_MOVED_DOWN
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
