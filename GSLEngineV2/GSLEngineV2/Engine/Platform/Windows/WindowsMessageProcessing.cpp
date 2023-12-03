#include "WindowsMessageProcessing.h"

LRESULT __stdcall EngineWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_CLOSE:
        PostQuitMessage(0 );
        return 0;
    }

    //消息返回Windows自己处理
    return DefWindowProc(hwnd , msg , wParam , lParam);
}
