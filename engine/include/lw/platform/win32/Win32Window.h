#pragma once
#include <windows.h>
#include <lw/graphics/RenderTarget.h>

namespace lw 
{

struct KeyboardInputState
{
    bool w = false;
    bool a = false;
    bool s = false;
    bool d = false;
    bool esc = false;
};

struct MouseInputState {
    LONG dx = 0;
    LONG dy = 0;
    bool isMouseLookActive = false;
};

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
    void Present(const Framebuffer& framebuffer);

    const KeyboardInputState& GetKeyboardInput() const { return keyboardInput; }
    const MouseInputState& GetMouseInput() const { return mouseInput; }

private:
    HWND m_hwnd;
    HINSTANCE m_hInstance;
    bool m_shouldClose;

    BITMAPINFO gBitmapInfo;
    KeyboardInputState keyboardInput;
    MouseInputState mouseInput;

    void ConfineCursorToClient();
    void SetMouseLook(bool enabled);

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    static void InitRawMouse(HWND hWnd);
    void InitBitmapInfo();
};
}