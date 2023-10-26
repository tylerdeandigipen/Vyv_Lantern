#include "EventBitmap.h"

void EventBitmap::SetEvent(int eventBit)
{
	bits |= (1 << eventBit);
}

void EventBitmap::ClearEvent(int eventBit)
{
	bits &= ~(1 << eventBit);
}

bool EventBitmap::CheckEvent(int eventBit) const
{
	return (bits & (1 << eventBit)) != 0;
}