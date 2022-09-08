#include "WindowsEngine.h"
#include "../../Debug/EngineDebug.h"
#include "../../Debug/Log/SimpleLog.h"
#include "../../Config/EngineRenderConfig.h"
#if defined(_WIN32)
#include "WindowsMessageProcessing.h"

int FWindowsEngine::PreInit(FWinMainCommandParameters InParameter)
{
    //0 初始化日志系统
    const char LogPath[] = "../log";
    init_log_system(LogPath);
    Engine_Log("Log Init");
    
    //  1 处理命名
    if (InitWindows(InParameter))
    {
        
    }

    if (InitDirect3D())
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
	FEngineRenderConfig::Destroy();
	Engine_Log("ExitWindowsEngine");
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

    RECT Rect = {
    	0,
    	0,
    	FEngineRenderConfig::GetRenderConfig()->ScreenHight,
    	FEngineRenderConfig::GetRenderConfig()->ScreenWidth
    };  // 分辨率

    ///@rect 视口
    ///WS_OVERLAPPEDWINDOW  视口风格
    ///NULL 没有菜单
    AdjustWindowRect(&Rect , WS_OVERLAPPEDWINDOW,NULL);

    int WindowWidth = Rect.right - Rect.left;
    int WindowHeight = Rect.bottom - Rect.top;

    MianWindowsHandle = CreateWindowEx(
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

    if (!MianWindowsHandle)
    {
        MessageBox(0,L"Create Window Failed" , 0 , 0);
        return false;
    }

    //显示窗口
    ShowWindow(MianWindowsHandle,SW_SHOW);
    UpdateWindow(MianWindowsHandle);
    
    return true;
}

bool FWindowsEngine::InitDirect3D()
{
    ////////////////////////////////////////////////////////////////////////////////////////
    //HRESULT
    //S_OK				0x00000000
    //E_UNEXPECTED		0x8000FFFF 意外的失败
    //E_NOTIMPL			0x80004001 未实现
    //E_OUTOFMEMORY		0x8007000E 未能分配所需的内存
    //E_INVALIDARG		0x80070057 一个或多个参数无效 
    //E_NOINTERFACE		0x80004002 不支持此接口
    //E_POINTER			0x80004003 无效指针
    //E_HANDLE			0x80070006 无效句柄
    //E_ABORT			0x80004004 操作终止
    //E_FAIL			0x80004005 错误
    //E_ACCESSDENIED	0x80070005 一般的访问被拒绝错误
    ANALYSIS_HRESULT(CreateDXGIFactory1(IID_PPV_ARGS(&DXGIFactory)));


    /*
    D3D_FEATURE_LEVEL_9_1  目标功能级别支持Direct3D 9.1包含 shader model 2.
    D3D_FEATURE_LEVEL_9_2  目标功能级别支持Direct3D 9.2包含 shader model 2.
    D3D_FEATURE_LEVEL_9_3  目标功能级别支持Direct3D 9.3包含 shader model 3.
    D3D_FEATURE_LEVEL_10_0 目标功能级别支持Direct3D 10.0包含 shader model 4.
    D3D_FEATURE_LEVEL_10_1 目标功能级别支持Direct3D 10.1包含 shader model 4.
    D3D_FEATURE_LEVEL_11_0 目标功能级别支持Direct3D 11.0包含 shader model 5.
    */

    HRESULT D3dDeviceResult = D3D12CreateDevice(NULL, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&D3dDevice));
    if (FAILED(D3dDeviceResult))
    {
        //warp
        ComPtr<IDXGIAdapter> WARPAdapter;
        ANALYSIS_HRESULT(DXGIFactory->EnumWarpAdapter(IID_PPV_ARGS(&WARPAdapter)));
        ANALYSIS_HRESULT(D3D12CreateDevice(WARPAdapter.Get(),D3D_FEATURE_LEVEL_11_0,IID_PPV_ARGS(&D3dDevice)));
    }

    //创建Fence对象 为了CPU和GPU同步作准备
    //D3D12_FENCE_FLAG_NONE  此参数主要决定是否共享 
    //D3D11_FENCE_FLAG_SHARED
    //D3D11_FENCE_FLAG_SHARED_CROSS_ADAPTER
    /*
    Fence->SetEventOnCompletion
    执行命令
    提交呈现
    Queue->Signal
    wait
    */
    ANALYSIS_HRESULT(D3dDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));
    
   //初始化命令对象
////////////////////////////////////////////////////////////////////////////////////////
	//INT Priority 
	//D3D12_COMMAND_QUEUE_PRIORITY
	//D3D12_COMMAND_QUEUE_PRIORITY_NORMAL
	//D3D12_COMMAND_QUEUE_PRIORITY_HIGH
	//NodeMask 指示该命令队列应在哪个GPU节点上执行
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;//直接执行
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAGS::D3D12_COMMAND_QUEUE_FLAG_NONE;
	ANALYSIS_HRESULT(D3dDevice->CreateCommandQueue(&QueueDesc,IID_PPV_ARGS(&CommandQueue)));

	ID3D12CommandAllocator Allocator();
	ANALYSIS_HRESULT(D3dDevice->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,
		IID_PPV_ARGS(CommandAllocator.GetAddressOf())));

	ANALYSIS_HRESULT(D3dDevice->CreateCommandList(
		0, //默认单个Gpu 
		D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT,//直接类型
		CommandAllocator.Get(),//将Commandlist关联到Allocator
		NULL,//ID3D12PipelineState  管线状态 如果没有会创建一个虚拟状态
		IID_PPV_ARGS(GraphicsCommandList.GetAddressOf())));

	GraphicsCommandList->Close();

	//多重采样
////////////////////////////////////////////////////////////////////
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS QualityLevels;
	QualityLevels.Format = BackBufferFormat;
	QualityLevels.SampleCount = 4;
	QualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVEL_FLAGS::D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	QualityLevels.NumQualityLevels = 0;

	ANALYSIS_HRESULT(D3dDevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&QualityLevels,
		sizeof(QualityLevels)));

	M4XQualityLevels = QualityLevels.NumQualityLevels;

	//交换链
////////////////////////////////////////////////////////////////////
	SwapChain.Reset();
	DXGI_SWAP_CHAIN_DESC SwapChainDesc;
	SwapChainDesc.BufferDesc.Width = FEngineRenderConfig::GetRenderConfig()->ScreenWidth;
	SwapChainDesc.BufferDesc.Height = FEngineRenderConfig::GetRenderConfig()->ScreenHight;
	SwapChainDesc.BufferDesc.RefreshRate.Numerator = FEngineRenderConfig::GetRenderConfig()->RefreshRate;
	SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	SwapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER::DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapChainDesc.BufferCount = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	//DXGI_USAGE_BACK_BUFFER 用于后台缓冲区
	//DXGI_USAGE_READ_ONLY  资源只读模式
	//DXGI_USAGE_SHADER_INPUT   作为输入
	//DXGI_USAGE_SHARED  共享资源
	//DXGI_USAGE_UNORDERED_ACCESS  无序访问
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;//使用表面或资源作为输出渲染目标。
	SwapChainDesc.OutputWindow = MianWindowsHandle;//指定windows句柄
	SwapChainDesc.Windowed = true;//以窗口运行
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_FLIP_DISCARD; //如何处理交换的BUFF 这里是丢弃 DX12好像只支持这个
	SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG::DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;//IDXGISwapChain::ResizeTarget 
	SwapChainDesc.BufferDesc.Format = BackBufferFormat;//纹理格式

	//多重采样设置
	SwapChainDesc.SampleDesc.Count = bMSAA4XEnabled ? 4 : 1;
	SwapChainDesc.SampleDesc.Quality = bMSAA4XEnabled ? (M4XQualityLevels - 1) : 0;
	ANALYSIS_HRESULT(DXGIFactory->CreateSwapChain(
		CommandQueue.Get(), 
		&SwapChainDesc, SwapChain.GetAddressOf()));

	//资源描述符
	////////////////////////////////////////////////////////////////////
	//D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV	//CBV常量缓冲区视图 SRV着色器资源视图 UAV无序访问视图
	//D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER		//采样器视图
	//D3D12_DESCRIPTOR_HEAP_TYPE_RTV			//渲染目标的视图资源
	//D3D12_DESCRIPTOR_HEAP_TYPE_DSV			//深度/模板的视图资源
	//RTV
	D3D12_DESCRIPTOR_HEAP_DESC RTVDescriptorHeapDesc;
	RTVDescriptorHeapDesc.NumDescriptors = FEngineRenderConfig::GetRenderConfig()->SwapChainCount;
	RTVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	RTVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	RTVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(
		&RTVDescriptorHeapDesc, 
		IID_PPV_ARGS(RTVHeap.GetAddressOf())));
	
	//DSV
	D3D12_DESCRIPTOR_HEAP_DESC DSVDescriptorHeapDesc;
	DSVDescriptorHeapDesc.NumDescriptors = 1;
	DSVDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	DSVDescriptorHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DSVDescriptorHeapDesc.NodeMask = 0;
	ANALYSIS_HRESULT(D3dDevice->CreateDescriptorHeap(
		&DSVDescriptorHeapDesc, 
		IID_PPV_ARGS(DSVHeap.GetAddressOf())));
	
	return false;
}
#endif
