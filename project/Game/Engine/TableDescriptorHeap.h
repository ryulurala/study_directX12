#pragma once

class TableDescriptorHeap
{
public:
	void Init(uint32 count);
	void Clear();
	
	void SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg);
	void SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg);

	void CommitTable();

	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const { return _descHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(CBV_REGISTER reg) const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(SRV_REGISTER reg) const;

private:
	D3D12_CPU_DESCRIPTOR_HANDLE GetCpuHandle(uint8 reg) const;

private:
	ComPtr<ID3D12DescriptorHeap> _descHeap;
	uint64 _handleSize = 0;		// = handleIncrementSize
	uint64 _groupSize = 0;
	uint64 _groupCount = 0;

	uint32 _currentGroupIndex = 0;

};
