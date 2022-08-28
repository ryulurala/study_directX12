#pragma once

#include <windows.h>
#include <tchar.h>
#include <memory>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <map>
using namespace std;

// DirectX 관련
#include "d3dx12.h"		// Microsoft의 비공식 라이브러리
#include <d3d12.h>
#include <wrl.h>
#include <d3dcompiler.h>
#include <dxgi.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

// Library
#pragma comment(lib, "d3d12")
#pragma comment(lib, "dxgi")
#pragma comment(lib, "dxguid")
#pragma comment(lib, "d3dcompiler")

// typedef
using int8		= __int8;
using int16		= __int16;
using int32		= __int32;
using int64		= __int64;
using uint8		= unsigned __int8;
using uint16	= unsigned __int16;
using uint32	= unsigned __int32;
using uint64	= unsigned __int64;
using Vec2		= XMFLOAT2;
using Vec3		= XMFLOAT3;
using Vec4		= XMFLOAT4;
using Matrix	= XMMATRIX;

enum
{
	SWAP_CHAIN_BUFFER_COUNT = 2,
};

struct WindowInfo
{
	HWND	hwnd;		// 출력 윈도우
	int32	width;		// 너비
	int32	height;		// 높이
	bool	windowed;	// 창 or 전체화면
};

class Engine;	// 전방 선언

extern unique_ptr<Engine> GEngine;	// 외부 전역 변수 선언

struct Vertex
{
	Vec3 pos;	// x, y, z
	Vec4 color;	// R, G, B, A
};