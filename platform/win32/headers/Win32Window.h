#pragma once
#include <windows.h>
#include "RenderTarget.h"

class Win32Window
{
public:

    float windowWidth;
    float windowHeight;

    Win32Window(HINSTANCE hInstance);
    ~Win32Window() = default;

    bool Create(int width, int height, const LPCWSTR title, int nCmdShow);
    void PollEvents();
    bool ShouldClose() const { return m_shouldClose; };

    HWND GetHandle() const { return m_hwnd; };
    void Present(const lw::Framebuffer& framebuffer);

private:
    HWND m_hwnd;
    HINSTANCE m_hInstance;
    bool m_shouldClose;

    BITMAPINFO gBitmapInfo;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    void InitBitmapInfo();
};