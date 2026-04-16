#pragma once
#include <windef.h>

class Camera;

class Renderer
{
private:
    HWND m_hWnd;

public:
    Renderer(HWND hWnd) : m_hWnd(hWnd) {}
    bool Initialize();
    void Render(const Camera& cam);
    void Cleanup();
};