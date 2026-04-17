#pragma once
#include <windows.h>

class Input
{
public:
    void Update()
    {
        prevMousePos = currMousePos;
        GetCursorPos(&currMousePos);

        mouseDeltaX = currMousePos.x - prevMousePos.x;
        mouseDeltaY = currMousePos.y - prevMousePos.y;

        wheelDelta = currentWheelDelta;
        currentWheelDelta = 0;
    }

    void OnMouseWheel(short delta)
    {
        currentWheelDelta += delta / 120.0f;
    }

    bool IsKeyDown(int key)
    {
        return (GetAsyncKeyState(key) & 0x8000) != 0;
    }

    bool IsMouseRightDown()
    {
        return IsKeyDown(VK_RBUTTON);
    }

    bool IsMouseLeftDown()
    {
        return IsKeyDown(VK_LBUTTON);
    }

    bool IsMouseMiddleDown()
    {
        return IsKeyDown(VK_MBUTTON);
    }

    bool IsAltDown()
    {
        return IsKeyDown(VK_MENU);
    }

    float GetMouseDeltaX() const { return (float)mouseDeltaX; }
    float GetMouseDeltaY() const { return (float)mouseDeltaY; }
    float GetMouseWheel() const { return wheelDelta; }

private:
    POINT prevMousePos{};
    POINT currMousePos{};

    int mouseDeltaX = 0;
    int mouseDeltaY = 0;

    float wheelDelta = 0;
    float currentWheelDelta = 0;
};