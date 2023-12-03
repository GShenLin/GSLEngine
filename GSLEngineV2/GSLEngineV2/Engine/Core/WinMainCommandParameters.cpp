#include "WinMainCommandParameters.h"

FWinMainCommandParameters::FWinMainCommandParameters(HINSTANCE InhInstance, HINSTANCE InprevInstance, PSTR IncmdLine,
    int InshowCmd)
{
    hInstance = InhInstance;
    prevInstance = InprevInstance;
    cmdLine = IncmdLine;
    showCmd = InshowCmd;
}
