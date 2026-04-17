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

    return true;
}

void Win32Window::PollEvents() {
  MSG msg = {};

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
            case 'W': window->input.w = true; break;
            case 'A': window->input.a = true; break;
            case 'S': window->input.s = true; break;
            case 'D': window->input.d = true; break;
            case VK_ESCAPE: window->input.esc = true; PostQuitMessage(0); break;
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
            case 'W': window->input.w = false; break;
            case 'A': window->input.a = false; break;
            case 'S': window->input.s = false; break;
            case 'D': window->input.d = false; break;
            }
        }
        return 0;
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