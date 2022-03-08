#pragma once
#include "Engine.h"

class FEngineFactory
{
   
public:
    FEngineFactory();
    static FEngine *  CreateEngine();
};


