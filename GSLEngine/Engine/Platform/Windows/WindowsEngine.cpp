#include "WindowsEngine.h"

#include "../../Debug/Log/SimpleLog.h"
#if defined(_WIN32)
#include "WindowsMessageProcessing.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameter)
{
    //0 初始化日志系统
    /*const char LogPath[] = "../log";
    init_log_system(LogPath);*/
    //Engine_Log("Log Init");
    
    //  1 处理命名
    if (InitWindows(InParameter))
    {
        
    }
    return 0;
}

int FWindowsEngine::Init()
{
    return 0;
}

int FWindowsEngine::PostInit()
{
    return 0;
    
}

void FWindowsEngine::Tick()
{
}

int FWindowsEngine::PreExit()
{
    return 0;
}

int FWindowsEngine::Exit()
{
    return 0;
}

int FWindowsEngine::PostExit()
{
    return 0;
}

bool FWindowsEngine::InitWindows(const FWinMainCommandParameters InParameter)
{
    //注册窗口
    WNDCLASSEX WindowsClass;
    WindowsClass.cbSize = sizeof(WNDCLASSEX); //该对象实际占用多大内存
    WindowsClass.cbClsExtra = 0; //是否需要额外空间
    WindowsClass.cbWndExtra = 0; // 是否需要额外内存
    WindowsClass.hbrBackground = nullptr; // 如果有设置就是GDI擦除
    WindowsClass.hCursor = LoadCursor(NULL,IDC_ARROW); //设置箭头光标
    WindowsClass.hIcon = nullptr; //应用程序放在磁盘上显示的图标
    WindowsClass.hIconSm = NULL;
    WindowsClass.hInstance = InParameter.hInstance; //窗口实例
    WindowsClass.lpszClassName = L"GSLEngine"; //窗口名字
    WindowsClass.lpszMenuName = nullptr; // 菜单名字 null不需要菜单
    WindowsClass.style = CS_VREDRAW | CS_HREDRAW; //如何绘制窗口 垂直和水平重绘
    WindowsClass.lpfnWndProc = EngineWindowProc; //消息层处理函数

    //注册窗口
    ATOM RegisterAtom = RegisterClassEx(&WindowsClass);  
    if (!RegisterAtom)
    {
        MessageBox(NULL, L"Register Fail , " , L"Error" , MB_OK);
    }

    RECT Rect = {0,0,1920,1080};  // 分辨率

    ///@rect 视口
    ///WS_OVERLAPPEDWINDOW  视口风格
    ///NULL 没有菜单
    AdjustWindowRect(&Rect , WS_OVERLAPPEDWINDOW,NULL);

    int WindowWidth = Rect.right - Rect.left;
    int WindowHeight = Rect.bottom - Rect.top;

    HWND hwnd = CreateWindowEx(
        NULL, //窗口风格
        L"GSLEngine",  //窗口类名称
        L"GSL Engine",  //显示名称
        WS_OVERLAPPEDWINDOW,  // 窗口风格
        0,  0, // 窗口初始坐标
        WindowWidth, WindowHeight, // 窗口大小
        NULL, //父窗口句柄
        nullptr,  //菜单句柄
        InParameter.hInstance,  //窗口实例
        NULL  //额外参数
    );

    if (!hwnd)
    {
        MessageBox(0,L"Create Window Failed" , 0 , 0);
        return false;
    }

    //显示窗口
    ShowWindow(hwnd,SW_SHOW);
    UpdateWindow(hwnd);
    
    return true;
}
#endif
