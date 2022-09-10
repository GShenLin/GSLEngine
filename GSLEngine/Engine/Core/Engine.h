#pragma once

#if defined(_WIN32)
#include "WinMainCommandParameters.h"
#endif


class FEngine
{
public:
    // 1 
    virtual int PreInit(
#if defined(_WIN32)
    FWinMainCommandParameters InParameter
#endif
        ) = 0;
    virtual int Init(
#if defined(_WIN32)
    FWinMainCommandParameters InParameter
#endif
    ) = 0 ;
    virtual int PostInit() = 0;
    
    virtual void Tick(float DeltaTime) = 0;

    virtual int PreExit() = 0;
    virtual int Exit() = 0;
    virtual int PostExit() = 0 ;
};
