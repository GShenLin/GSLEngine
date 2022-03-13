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
};

#endif