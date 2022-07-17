#pragma once


class SwapChain
{
public:
	void Init(const WindowInfo& info, ComPtr<IDXGIFactory> dxgi, ComPtr<ID3D12CommandQueue> cmdQueue);
	
	void Present();
	void SwapIndex();
	
	ComPtr<IDXGISwapChain> GetSwapChain() const { return _swapChain; }
	ComPtr<ID3D12Resource> GetRenderTarget(int32 index) const { return _renderTargets[index]; }

	uint32 GetCurrentBackBufferIndex() const { return _backBufferIndex; }
	ComPtr<ID3D12Resource> GetCurrentBackBufferResource() const { return _renderTargets[_backBufferIndex]; }

private:
	ComPtr<IDXGISwapChain>	_swapChain;
	ComPtr<ID3D12Resource>	_renderTargets[SWAP_CHAIN_BUFFER_COUNT];	// EnginePch에 매크로 선언
	uint32					_backBufferIndex = 0;						// 0 or 1

};

