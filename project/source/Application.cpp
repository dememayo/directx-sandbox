#include "pch.h"
#include "Application.h"
#include "Renderer.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

Application::Application(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
        : m_hInstance(hInstance), m_hPrevInstance(hPrevInstance), m_lpCmdLine(lpCmdLine), m_nCmdShow(nCmdShow)
{

}

Application::~Application()
{

}

int Application::Run(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
    std::unique_ptr<Application> app = std::unique_ptr<Application>(new Application(hInstance, hPrevInstance, lpCmdLine, nCmdShow));
    if (!app->Initialize())
    {
        return 0;
    }
    int exitCode = app->MainLoop();
    app->Cleanup();

    return exitCode;
}

bool Application::Initialize()
{
    if (!InitWindow()) return false;

    m_Renderer = std::unique_ptr<Renderer>(new Renderer(m_hWnd));

    if (!m_Renderer->Initialize())
    {
        m_Renderer->Cleanup();
        return false;
    }

    return true;
}

bool Application::InitWindow()
{
    // Register class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = m_hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = L"TutorialWindowClass";
    wcex.hIconSm = nullptr;
    if (!RegisterClassEx(&wcex)) return false;

    // Create window
    RECT rc = {0, 0, 800, 600};
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
    HWND hWnd = CreateWindow(L"TutorialWindowClass", L"VSCode + DirectX",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, rc.right - rc.left, rc.bottom - rc.top, nullptr, nullptr, m_hInstance,
        nullptr
    );
    if (!hWnd) return false;

    ShowWindow(hWnd, m_nCmdShow);
    m_hWnd = hWnd;

    return true;
}

int Application::MainLoop()
{
    // Main message loop
    MSG msg = {0};
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_Renderer->Render();
        }
    }
    return (int)msg.wParam;
}

void Application::Cleanup()
{
    m_Renderer->Cleanup();
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;

    switch(message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE)
        {
            DestroyWindow(hWnd);
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}