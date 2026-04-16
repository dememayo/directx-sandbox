#pragma once
#include <windows.h>

class Timer
{
public:
    void Init()
    {
        QueryPerformanceFrequency(&frequency);
        QueryPerformanceCounter(&prev);
    }

    float Tick()
    {
        LARGE_INTEGER now;
        QueryPerformanceCounter(&now);

        float deltaTime = float(now.QuadPart - prev.QuadPart) / frequency.QuadPart;
        prev = now;

        return deltaTime;
    }

private:
    LARGE_INTEGER frequency{};
    LARGE_INTEGER prev{};
};