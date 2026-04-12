#pragma once
#include <windef.h>

class Renderer
{
private:
    HWND m_hWnd;

public:
    Renderer(HWND hWnd) : m_hWnd(hWnd) {}
    bool Initialize();
    void Render();
    void Cleanup();
};