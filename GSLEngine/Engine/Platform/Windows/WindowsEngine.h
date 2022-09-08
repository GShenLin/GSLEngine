#pragma once

#if defined(_WIN32)
#include "../../Core/Engine.h"

class FWindowsEngine : public FEngine
{
public:
    int PreInit(FWinMainCommandParameters InParameter) override;
    int Init() override;
    int PostInit() override;
    void Tick() override;
    int PreExit() override;
    int Exit() override;
    int PostExit() override;

private:
    bool InitWindows(const FWinMainCommandParameters InParameter);

    bool InitDirect3D();

protected:
    ComPtr<IDXGIFactory4> DXGIFactory;//创建 DirectX 图形基础结构 (DXGI) 对象
    ComPtr<ID3D12Device> D3dDevice;//创建命令分配器、命令列表、命令队列、Fence、资源、管道状态对象、堆、根签名、采样器和许多资源视图
    ComPtr<ID3D12Fence> Fence;//一个用于同步 CPU 和一个或多个 GPU 的对象。

    ComPtr<ID3D12CommandQueue> CommandQueue;//队列
    ComPtr<ID3D12CommandAllocator> CommandAllocator; //存储
    ComPtr<ID3D12GraphicsCommandList> GraphicsCommandList;//命令列表

    ComPtr<IDXGISwapChain> SwapChain;

    //描述符对象和堆
    ComPtr<ID3D12DescriptorHeap> RTVHeap;
    ComPtr<ID3D12DescriptorHeap> DSVHeap;

    vector<ComPtr<ID3D12Resource>> SwapChainBuffer;
    ComPtr<ID3D12Resource> DepthStencilBuffer;

    //和屏幕的视口有关
    D3D12_VIEWPORT ViewprotInfo;
    D3D12_RECT ViewprotRect;

protected:
    HWND MianWindowsHandle;//主windows句柄
    UINT M4XQualityLevels;
    bool bMSAA4XEnabled;
    DXGI_FORMAT BackBufferFormat;
    DXGI_FORMAT DepthStencilFormat;
    UINT RTVDescriptorSize;
};

#endif