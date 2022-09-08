#pragma once
#include "../EngineMinimal.h"
#if 1

class FWinMainCommandParameters
{
public:
    FWinMainCommandParameters(HINSTANCE InhInstance, HINSTANCE InprevInstance,PSTR IncmdLine, int InshowCmd);

    //通过这种方式保存一下参数
    HINSTANCE hInstance;
    HINSTANCE prevInstance;
    PSTR cmdLine;
    int showCmd;
};
#elif

#endif