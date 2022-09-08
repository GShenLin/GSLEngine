#pragma once

struct FEngineRenderConfig
{
    FEngineRenderConfig();

    int ScreenWidth;
    int ScreenHight;
    int RefreshRate;
    int SwapChainCount;

    static FEngineRenderConfig* GetRenderConfig();
    static void Destroy();
private:
    static FEngineRenderConfig* RenderConfig;
};