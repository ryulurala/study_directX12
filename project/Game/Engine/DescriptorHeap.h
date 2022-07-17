#pragma once

class SwapChain;

class DescriptorHeap
{
public:
	void Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain);

	D3D12_CPU_DESCRIPTOR_HANDLE GetRTV(int32 idx) const { return _rtvHandle[idx]; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetBackBufferView() const;

private:
	// Render Target View
	ComPtr<ID3D12DescriptorHeap>	_rtvHeap;
	uint32							_rtvHeapSize;
	D3D12_CPU_DESCRIPTOR_HANDLE		_rtvHandle[SWAP_CHAIN_BUFFER_COUNT];

	shared_ptr<SwapChain>		_swapChain;
};

