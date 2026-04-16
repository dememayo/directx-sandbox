#include "pch.h"
#include "Application.h"
#include "Renderer.h"
#include "Camera.h"
#include "Input.h"
#include "Timer.h"

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
    m_Input = std::make_unique<Input>();

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
    wcex.lpfnWndProc = StaticWndProc;
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
        this
    );
    if (!hWnd) return false;

    ShowWindow(hWnd, m_nCmdShow);
    m_hWnd = hWnd;

    return true;
}

void UpdateCamera(Camera& cam, float dt, Input& input)
{
    float rotSpeed = 0.01f;
    float panSpeed = 0.01f;
    float moveSpeed = cam.moveSpeed * dt;

    if (input.IsAltDown() && input.IsMouseLeftDown())
    {
        cam.yaw   += input.GetMouseDeltaX() * rotSpeed;
        cam.pitch -= input.GetMouseDeltaY() * rotSpeed;

        cam.pitch = std::clamp(cam.pitch, -XM_PIDIV2 + 0.1f, XM_PIDIV2 - 0.1f);
    }

    if (input.IsMouseMiddleDown())
    {
        XMVECTOR right = XMVectorSet(sinf(cam.yaw - XM_PIDIV2), 0, cosf(cam.yaw - XM_PIDIV2), 0);
        XMVECTOR up = XMVectorSet(0,1,0,0);

        XMVECTOR move = right * (-input.GetMouseDeltaX() * panSpeed) + up * (input.GetMouseDeltaY() * panSpeed);
        XMVECTOR tgt = XMLoadFloat3(&cam.target);
        tgt += move;

        XMStoreFloat3(&cam.target, tgt);
    }

    cam.distance -= input.GetMouseWheel() * 0.5f;
    cam.distance = std::max<float>(cam.distance, 0.1f);

    XMVECTOR offset = XMVectorSet(
        cosf(cam.pitch) * sinf(cam.yaw),
        sinf(cam.pitch),
        cosf(cam.pitch) * cosf(cam.yaw),
        0
    );

    offset *= cam.distance;

    XMVECTOR tgt = XMLoadFloat3(&cam.target);
    XMVECTOR pos = tgt - offset;

    XMStoreFloat3(&cam.position, pos);

    if (input.IsMouseRightDown())
    {
        cam.yaw   += input.GetMouseDeltaX() * cam.mouseSensitivity;
        cam.pitch -= input.GetMouseDeltaY() * cam.mouseSensitivity;

        cam.pitch = std::clamp(cam.pitch, -XM_PIDIV2 + 0.1f, XM_PIDIV2 - 0.1f);

        XMVECTOR forward = XMVectorSet(
            sinf(cam.yaw),
            0,
            cosf(cam.yaw),
            0
        );

        XMVECTOR right = XMVector3Cross(
            XMVectorSet(0,1,0,0),
            forward
        );

        XMVECTOR pos = XMLoadFloat3(&cam.position);

        if (input.IsKeyDown('W')) pos += forward * moveSpeed;
        if (input.IsKeyDown('S')) pos -= forward * moveSpeed;
        if (input.IsKeyDown('A')) pos -= right * moveSpeed;
        if (input.IsKeyDown('D')) pos += right * moveSpeed;

        XMVECTOR tgt2 = XMLoadFloat3(&cam.target);

        XMVECTOR delta = pos - XMLoadFloat3(&cam.position);
        tgt2 += delta;

        XMStoreFloat3(&cam.position, pos);
        XMStoreFloat3(&cam.target, tgt2);
    }
}

int Application::MainLoop()
{
    // Main message loop
    MSG msg = {0};
    Camera cam;
    Timer timer;
    timer.Init();
    while (WM_QUIT != msg.message)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            m_Input->Update();
            float dt = timer.Tick();
            UpdateCamera(cam, dt, *m_Input);
            m_Renderer->Render(cam);
        }
    }
    return (int)msg.wParam;
}

void Application::Cleanup()
{
    m_Renderer->Cleanup();
}

LRESULT CALLBACK Application::StaticWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    Application* app = nullptr;

    if (msg == WM_NCCREATE)
    {
        CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
        app = reinterpret_cast<Application*>(cs->lpCreateParams);

        SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)app);
    }
    else
    {
        app = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    }

    if (app)
    {
        return app->WndProc(hWnd, msg, wParam, lParam);
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}

LRESULT Application::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    short delta = GET_WHEEL_DELTA_WPARAM(wParam);

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
    case WM_MOUSEWHEEL:
        m_Input->OnMouseWheel(delta);
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }

    return 0;
}