#pragma once


class SwapChain
{
public:
	void Init(const WindowInfo& info, ComPtr<ID3D12Device> device, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	
	void Present();
	void SwapIndex();
	
	ComPtr<IDXGISwapChain> GetSwapChain() const { return _swapChain; }
	ComPtr<ID3D12Resource> GetRenderTarget(int32 index) const { return _rtvBuffer[index]; }

	ComPtr<ID3D12Resource> GetBackRTVBuffer() const { return _rtvBuffer[_backBufferIndex]; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetBackRTV() const { return _rtvHandle[_backBufferIndex]; }

private:
	void CreateSwapChain(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	void CreateRTV(ComPtr<ID3D12Device> device);

private:
	ComPtr<IDXGISwapChain>	_swapChain;
	ComPtr<ID3D12Resource>	_rtvBuffer[SWAP_CHAIN_BUFFER_COUNT];	// EnginePch에 매크로 선언
	uint32					_backBufferIndex = 0;						// 0 or 1

	// Render Target View
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];
};

