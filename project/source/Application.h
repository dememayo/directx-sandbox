#pragma once
#include <windef.h>

class Renderer;
class Input;

class Application
{
private:
    HINSTANCE m_hInstance;
    HINSTANCE m_hPrevInstance;
    LPWSTR m_lpCmdLine;
    int m_nCmdShow;
    HWND m_hWnd;
    std::unique_ptr<Renderer> m_Renderer;
    std::unique_ptr<Input> m_Input;

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
    LRESULT Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
    static LRESULT CALLBACK StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
};