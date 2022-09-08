#pragma once
#include "../simple_library/public/simple_library.h"
#include "Debug/Log/SimpleLog.h"

#pragma commet(lib , "d3dcompiler.lib")
#pragma commet(lib , "D3D12.lib")
#pragma commet(lib , "dxgi.lib")

#include <wrl.h> // Windows提供的库 智能指针 事件之类的
#include <dxgi1_4.h>   // 东西很多 主要有很多工厂 生成适配器之类的
#include <d3d12.h> //  D3D12 API
#include <D3Dcompiler.h>
#include <DirectXMath.h> // 数学库
#include <DirectXPackedVector.h> // 向量的库
#include <DirectXColors.h>  // 颜色相关库
#include <DirectXCollision.h>

using namespace Microsoft::WRL;