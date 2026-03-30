#pragma once
#include <windef.h>

class Renderer;

class Application
{
private:
    HINSTANCE m_hInstance;
    HINSTANCE m_hPrevInstance;
    LPWSTR m_lpCmdLine;
    int m_nCmdShow;
    HWND m_hWnd;
    std::unique_ptr<Renderer> m_Renderer;

public:
    static int Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);

public:
    Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow);
    ~Application();

private:
    bool Initialize();
    bool InitWindow();
    int MainLoop();
    void Cleanup();
};