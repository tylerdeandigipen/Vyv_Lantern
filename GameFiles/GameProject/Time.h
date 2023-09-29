#pragma once
#ifndef TIME_H
#define TIME_H

class Time
{
public:
    Time();
    ~Time();
    float Delta();
private:
    float last_tick_time;
    float delta;
};

#endif TIME_H
