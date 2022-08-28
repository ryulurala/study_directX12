#include "pch.h"
#include "SwapChain.h"

void SwapChain::Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
{
	CreateSwapChain(info, dxgi, cmdQueue);
	CreateRTV(device);
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

void SwapChain::CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue)
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
		_swapChain->GetBuffer(i, IID_PPV_ARGS(&_rtvBuffer[i]));
}

void SwapChain::CreateRTV(ComPtr<ID3D12Device> device)
{
	// RTV(Render Target View) 크기 계산
	int32 rtvHeapSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	D3D12_DESCRIPTOR_HEAP_DESC rtvDesc;
	rtvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDesc.NumDescriptors = SWAP_CHAIN_BUFFER_COUNT;
	rtvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvDesc.NodeMask = 0;

	// 이 시점에 rtvHeap 배열 생성
	device->CreateDescriptorHeap(&rtvDesc, IID_PPV_ARGS(&_rtvHeap));

	// rtvHeap의 시작 주소 값을 가져오기
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapBegin = _rtvHeap->GetCPUDescriptorHandleForHeapStart();

	for (int i = 0; i < SWAP_CHAIN_BUFFER_COUNT; i++)
	{
		// "CD~"(from. d3dx12.h)
		// RTV 배열: [_rtvHeapBegin] [_rtvHeapBegin + (i * _rtvHeapSize)]
		_rtvHandle[i] = CD3DX12_CPU_DESCRIPTOR_HANDLE(rtvHeapBegin, i * rtvHeapSize);

		// Descriptor Heap으로 rtv(Render Target View) 생성
		device->CreateRenderTargetView(_rtvBuffer[i].Get(), nullptr, _rtvHandle[i]);
	}
}