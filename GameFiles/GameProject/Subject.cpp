#include "Subject.h"
#include "Observer.h"

void Subject::addObserver(Observer* observer)
{
    observer->observerNext = head_;
    head_ = observer;
}

void Subject::removeObserver(Observer* observer)
{
    if (head_ == observer) 
    {
        head_ = observer->observerNext;
        observer->observerNext = nullptr;
        return;
    }

    Observer* current = head_;
    while (current != nullptr) {
        if (current->observerNext == observer) {
            current->observerNext = observer->observerNext;
            observer->observerNext = nullptr;
            return;
        }
        current = current->observerNext;
    }
}

void Subject::notify(const Entity& entity, EventType event)
{
    Observer* observer = head_;

    while (observer != nullptr) 
    {
        observer->onNotify(entity, event);
        observer = observer->observerNext;
    }
}