#pragma once

constexpr int EVENT_MOVE = 0;  // Event bit for move event

class EventBitmap
{
public:
    EventBitmap() : bits(0) {}

    void SetEvent(int eventBit);
    void ClearEvent(int eventBit);
    bool CheckEvent(int eventBit) const;

private:
    int bits;
};
