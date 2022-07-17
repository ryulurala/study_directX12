#include "pch.h"
#include "SwapChain.h"

void SwapChain::Init(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	// 이전 내용은 Clear
	_swapChain.Reset();
	
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = static_cast<uint32>(info.width);		// 버퍼의 해상도 너비
	sd.BufferDesc.Height = static_cast<uint32>(info.height);	// 버퍼의 해상도 높이
	sd.BufferDesc.RefreshRate.Numerator = 60;					// 화면 갱신 비율 분자 값
	sd.BufferDesc.RefreshRate.Denominator = 1;					// 화면 갱신 비율 분모 값
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			// 버퍼의 디스플레이 방식
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;									// 멀티 샘플링 OFF
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;			// 후면 버퍼에 렌더링 출력
	sd.BufferCount = SWAP_CHAIN_BUFFER_COUNT;					// 전면, 후면 버퍼(2개)
	sd.OutputWindow = info.hwnd;
	sd.Windowed = info.windowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;				// 전면, 후면 버퍼 교체 시 이전 프레임 정보 버림
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	// 출력과 관련되어서 Device가 아닌 DXGI에서 생성해준다.
	// 동시에 버퍼도 생성.
	dxgi->CreateSwapChain(cmdQueue.Get(), &sd, &_swapChain);

	// 앞서 생성한 버퍼를 _renderTargets에 저장
	for (int32 i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_renderTargets[i]));
}

void SwapChain::Present()
{
	// 프레임 제공
	_swapChain->Present(0, 0);
}

void SwapChain::SwapIndex()
{
	_backBufferIndex = (_backBufferIndex + 1) % SWAP_CHAIN_BUFFER_COUNT;
}
