#include "EngineFactory.h"

#if defined(_WIN32)
#include "../Platform/Windows/WindowsEngine.h"
#elif 1

#endif

FEngineFactory::FEngineFactory()
{
}

FEngine* FEngineFactory::CreateEngine()
{
#if defined(_WIN32)
    return new FWindowsEngine();
#elif 1
    
#endif
}

