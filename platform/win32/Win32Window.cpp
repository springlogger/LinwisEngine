#include "Win32Window.h"
#include "LinwisEngine.h"

Win32Window::Win32Window(HINSTANCE hInstance) : m_hInstance(hInstance) {}

bool Win32Window::Create(int width, int height, LPCWSTR title, int nCmdShow) {

    const wchar_t CLASS_NAME[] = L"MyEngineWindowClass";
    gBitmapInfo = {};

    WNDCLASSW wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = m_hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);

    RegisterClassW(&wc);

    HWND hwnd = CreateWindowExW(
        0,
        CLASS_NAME,
        title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        width, height,
        nullptr,
        nullptr,
        m_hInstance,
        this
    );

    if (!hwnd)
        return false;

    m_hwnd = hwnd;

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    windowWidth = width;
    windowHeight = height;

    InitBitmapInfo();
    InitRawMouse(hwnd);

    return true;
}

void Win32Window::InitRawMouse(HWND hWnd)
{
    RAWINPUTDEVICE rid{};
    rid.usUsagePage = 0x01;
    rid.usUsage = 0x02;
    rid.dwFlags = 0;
    rid.hwndTarget = hWnd;

    RegisterRawInputDevices(&rid, 1, sizeof(rid));
}

void Win32Window::PollEvents() {
    MSG msg = {};

    mouseInput.dx = 0;
    mouseInput.dy = 0;

    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
        if (msg.message == WM_QUIT)
        {
            m_shouldClose = true;
        }

        TranslateMessage(&msg);
        DispatchMessage(&msg);
        }
}

void Win32Window::Present(const lw::Framebuffer& framebuffer)
{

    HDC hdc = GetDC(m_hwnd);

    StretchDIBits(
        hdc,
        0, 0, framebuffer.width, framebuffer.height,
        0, 0, framebuffer.width, framebuffer.height,
        framebuffer.pixels.data(),
        &gBitmapInfo,
        DIB_RGB_COLORS,
        SRCCOPY
    );

    ReleaseDC(m_hwnd, hdc);
}

void Win32Window::ConfineCursorToClient()
{
    RECT rect{};
    GetClientRect(m_hwnd, &rect);

    POINT leftTop{ rect.left, rect.top };
    POINT rightBottom{ rect.right, rect.bottom };

    ClientToScreen(m_hwnd, &leftTop);
    ClientToScreen(m_hwnd, &rightBottom);

    RECT clipRect{
        leftTop.x,
        leftTop.y,
        rightBottom.x,
        rightBottom.y
    };

    ClipCursor(&clipRect);
}

void Win32Window::SetMouseLook(bool enabled)
{
    if (mouseInput.isMouseLookActive == enabled) {
        return;
    }

    mouseInput.isMouseLookActive = enabled;

    if (enabled)
    {
        ShowCursor(FALSE);
        SetCapture(m_hwnd);
        ConfineCursorToClient();

        mouseInput.dx = 0;
        mouseInput.dy = 0;
    }
    else
    {
        ShowCursor(TRUE);
        ReleaseCapture();
        ClipCursor(nullptr);

        mouseInput.dx = 0;
        mouseInput.dy = 0;
    }
}

LRESULT CALLBACK Win32Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

    Win32Window* window = nullptr;

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCTW* cs = reinterpret_cast<CREATESTRUCTW*>(lParam);
        window = static_cast<Win32Window*>(cs->lpCreateParams);
        SetWindowLongPtrW(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(window));
    }
    else
    {
        window = reinterpret_cast<Win32Window*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
    }

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        BeginPaint(hwnd, &ps);
        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_KEYDOWN:
    {
        if (window)
        {
            switch (wParam)
            {
            case 'W': window->keyboardInput.w = true; break;
            case 'A': window->keyboardInput.a = true; break;
            case 'S': window->keyboardInput.s = true; break;
            case 'D': window->keyboardInput.d = true; break;
            case VK_ESCAPE: {
                window->keyboardInput.esc = true; 

                if (window->mouseInput.isMouseLookActive) {
                    window->SetMouseLook(false);
                }
                else {
                    PostQuitMessage(0); 
                    break;
                }
            }
            }
        }
        return 0;
    }

    case WM_KEYUP:
    {
        if (window)
        {
            switch (wParam)
            {
            case 'W': window->keyboardInput.w = false; break;
            case 'A': window->keyboardInput.a = false; break;
            case 'S': window->keyboardInput.s = false; break;
            case 'D': window->keyboardInput.d = false; break;
            }
        }
        return 0;
    }

    case WM_LBUTTONDOWN: {
        if (window) {
            window->SetMouseLook(true);
        }
        return 0;
    }

    case WM_INPUT: {
        UINT dataSize = 0;
        GetRawInputData((HRAWINPUT)lParam, RID_INPUT, nullptr, &dataSize, sizeof(RAWINPUTHEADER));

        if (dataSize > 0) {
            std::vector<BYTE> buffer(dataSize);

            if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, buffer.data(), &dataSize, sizeof(RAWINPUTHEADER)) == dataSize) {
                RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(buffer.data());

                if (raw->header.dwType == RIM_TYPEMOUSE) {
                    window->mouseInput.dx += raw->data.mouse.lLastX;
                    window->mouseInput.dy += raw->data.mouse.lLastY;
                }
            }
        }

        break;
    }
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Win32Window::InitBitmapInfo()
{
    ZeroMemory(&gBitmapInfo, sizeof(gBitmapInfo));
    gBitmapInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    gBitmapInfo.bmiHeader.biWidth = windowWidth;
    gBitmapInfo.bmiHeader.biHeight = -windowHeight; // отрицательное значение = top-down bitmap
    gBitmapInfo.bmiHeader.biPlanes = 1;
    gBitmapInfo.bmiHeader.biBitCount = 32;
    gBitmapInfo.bmiHeader.biCompression = BI_RGB;
}