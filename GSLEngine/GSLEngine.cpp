#include "EngineMinimal.h"
#include "Engine/Core/EngineFactory.h"

//hInstance  自己的实例
//prevInstance  上次启动的实例
//cmdLine 传递命令行
//showCmd 命令数量
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,PSTR cmdLine, int showCmd)
{
    FEngine * EngineInstance = FEngineFactory::CreateEngine();
    int EngineReturnValue = 0;
    if (EngineInstance)
    {
#if defined(_WIN32)
        FWinMainCommandParameters InParameters(hInstance , prevInstance , cmdLine , showCmd);
        EngineReturnValue = EngineInstance->PreInit(InParameters);
#elif 1
        
#endif
        
        if (EngineReturnValue != 0)
        {
            return EngineReturnValue;
        }
        
        EngineInstance->Init();
        if (EngineReturnValue != 0)
        {
            return EngineReturnValue;
        }
        
        EngineInstance->PostInit();
        if (EngineReturnValue != 0)
        {
            return EngineReturnValue;
        }
        
        while (true)
        {
            EngineInstance->Tick();
        }

        EngineInstance->PreExit();
        if (EngineReturnValue != 0)
        {
            return EngineReturnValue;
        }
        
        EngineInstance->Exit();
        if (EngineReturnValue != 0)
        {
            return EngineReturnValue;
        }
        
        EngineInstance->PostExit();
        if (EngineReturnValue != 0)
        {
            return EngineReturnValue;
        }
        
        return 0;
    }
   
    return 1;
}
