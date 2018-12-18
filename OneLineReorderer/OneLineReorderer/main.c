/* Made by Baruch Rutman (peterooch at gmail dot com), see README */
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define IDC_REORDER 10
#define IDC_LAYOUT  11
HINSTANCE hInst;
HWND MainWnd, hInput, hOutput, hBtnReorder, hBtnLayout;
LPCWSTR ClassName = L"OneLineReorderer";
HDC hdc;
BOOL bRTL = 1;

static VOID Reorder()
{
    WCHAR szInput[MAX_PATH], szOutput[MAX_PATH];
    INT iInputLength;
    GCP_RESULTSW gcpResult;
    //HGLOBAL hMem;

    ZeroMemory(&gcpResult, sizeof(gcpResult));
    ZeroMemory(szInput,    sizeof(szInput));
    ZeroMemory(szOutput,   sizeof(szOutput));

    gcpResult.lpOutString = szOutput;
    gcpResult.nGlyphs     = MAX_PATH;
    gcpResult.lStructSize = sizeof(gcpResult);

    iInputLength = GetWindowTextW(hInput, szInput, MAX_PATH);

    GetCharacterPlacementW(hdc, szInput, iInputLength, 0, &gcpResult, GCP_REORDER);

    SetWindowTextW(hOutput, szOutput);
    UpdateWindow(hBtnLayout);

#if 0 // Works but make the program unusable...
    if (OpenClipboard(MainWnd))
    {
        if (EmptyClipboard())
        {
            hMem = GlobalAlloc(GMEM_MOVEABLE, sizeof(szOutput));
            CopyMemory(GlobalLock(hMem), szOutput, sizeof(szOutput));
            GlobalUnlock(hMem);
            SetClipboardData(CF_UNICODETEXT, hMem);
        }
        CloseClipboard();
    }
#endif
}

static VOID PaintWnd()
{
    TEXTMETRICW tm;
    PAINTSTRUCT ps;
    HFONT hfont;

    hdc   = BeginPaint(MainWnd, &ps);
    hfont = CreateFontW(0, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"Sans Serif");

    hInput      = CreateWindowExW(0, L"edit", L"", WS_CHILD | WS_BORDER | ES_LEFT, 10, 20, 500, 26, MainWnd, NULL, hInst, NULL);
    hOutput     = CreateWindowExW(0, L"edit", L"", WS_CHILD | WS_BORDER | ES_LEFT | ES_READONLY, 10, 80, 500, 26, MainWnd, NULL, hInst, NULL);
    hBtnReorder = CreateWindowExW(0, L"button", L"Reorder", WS_CHILD | BS_PUSHBUTTON | WS_BORDER, 10, 50, 70, 26, MainWnd, (HMENU)IDC_REORDER, hInst, NULL);
    hBtnLayout  = CreateWindowExW(0, L"button", L"", WS_CHILD | BS_PUSHBUTTON | WS_BORDER, 100, 50, 120, 26, MainWnd, (HMENU)IDC_LAYOUT, hInst, NULL);

    if (!hInput || !hOutput || !hBtnReorder || !hBtnLayout)
        return;

    if (hfont)
    {
        SelectObject(hdc, hfont);
        SendMessageW(hInput,      WM_SETFONT, (WPARAM)hfont, 0);
        SendMessageW(hOutput,     WM_SETFONT, (WPARAM)hfont, 0);
        SendMessageW(hBtnReorder, WM_SETFONT, (WPARAM)hfont, 0);
        SendMessageW(hBtnLayout,  WM_SETFONT, (WPARAM)hfont, 0);
    }

    SetWindowTextW(hBtnLayout, bRTL ? L"Switch to LTR" : L"Switch to RTL");
    ShowWindow(hInput,      SW_SHOW);
    ShowWindow(hOutput,     SW_SHOW);
    ShowWindow(hBtnReorder, SW_SHOW);
    ShowWindow(hBtnLayout,  SW_SHOW);
    GetTextMetricsW(hdc, &tm);
    TextOutW(hdc, 10, 20 - 2 - tm.tmAscent, L"Input Text", 10);
    TextOutW(hdc, 10, 106, L"Reordered Text", 14);
}

static LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
            PaintWnd();
            return TRUE;

        case WM_COMMAND:
            if (wParam == IDC_REORDER)
            {
                Reorder();
                return TRUE;
            }
            else if (wParam == IDC_LAYOUT)
            {
                SetLayout(hdc, bRTL ? 0 : LAYOUT_RTL);
                bRTL = !bRTL;
                SetWindowTextW(hBtnLayout, bRTL ? L"Switch to LTR" : L"Switch to RTL");
                return TRUE;
            }
            break;

        case WM_CLOSE:
        case WM_DESTROY:
            PostQuitMessage(0);
            return FALSE;
    }
    return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(
    HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPWSTR lpCmdLine,
    int nCmdShow)
{
    WNDCLASSEXW wc;
    MSG msg;

    hInst = hInstance;

    ZeroMemory(&wc, sizeof(wc));

    wc.lpfnWndProc   = MainWndProc;
    wc.hInstance     = hInst;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.lpszClassName = ClassName;
    wc.cbSize        = sizeof(wc);
    wc.style         = CS_HREDRAW | CS_VREDRAW;

    if (!RegisterClassExW(&wc))
        return 0;

    MainWnd = CreateWindowExW(WS_EX_LAYOUTRTL,
                              ClassName,
                              L"One Line Reorderer - Buggy as hell!",
                              WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                              CW_USEDEFAULT,
                              CW_USEDEFAULT,
                              600,
                              175,
                              NULL,
                              NULL,
                              hInst,
                              NULL);

    if (!MainWnd)
        return 0;

    ShowWindow(MainWnd, SW_SHOW);
    UpdateWindow(MainWnd);

    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    UnregisterClassW(ClassName, hInst);
    return 0;
}
