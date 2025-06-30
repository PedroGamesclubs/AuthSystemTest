#include <windows.h>
#include "AuthManager.hpp"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

HWND hInput, hButton, hStatus;

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE, LPSTR, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = L"Auth";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowW(L"Auth", L"Teste", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200, nullptr, nullptr, hInst, nullptr);

    ShowWindow(hwnd, nCmdShow);

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

void OnAuthClick() {
    wchar_t id[128];
    GetWindowTextW(hInput, id, 128);
    std::wstring wID(id);
    std::string status = AuthManager::authenticate(wID);
    SetWindowTextA(hStatus, status.c_str());
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        hInput = CreateWindowW(L"EDIT", L"", WS_CHILD | WS_VISIBLE | WS_BORDER,
            20, 20, 240, 25, hwnd, nullptr, nullptr, nullptr);

        hButton = CreateWindowW(L"BUTTON", L"Autenticar", WS_CHILD | WS_VISIBLE,
            20, 60, 240, 25, hwnd, (HMENU)1, nullptr, nullptr);

        hStatus = CreateWindowW(L"STATIC", L"Aguardando...", WS_CHILD | WS_VISIBLE,
            20, 100, 240, 25, hwnd, nullptr, nullptr, nullptr);
        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) OnAuthClick();
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
