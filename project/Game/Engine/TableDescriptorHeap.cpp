#include "pch.h"
#include "TableDescriptorHeap.h"

#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"

void TableDescriptorHeap::Init(uint32 count)
{
	_groupCount = count;
	
	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.NumDescriptors = count * REGISTER_COUNT;
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;	// for. GPU 메모리 상주
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;		// CBV 용도

	DEVICE->CreateDescriptorHeap(&desc, IID_PPV_ARGS(&_descHeap));

	_handleSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	_groupSize = _handleSize * REGISTER_COUNT;
}

void TableDescriptorHeap::Clear()
{
	_currentGroupIndex = 0;
}

void TableDescriptorHeap::SetCBV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, CBV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCpuHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;

	// Desc.Heap(CBV) -> Desc.Heap(Shader Visible)
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TableDescriptorHeap::SetSRV(D3D12_CPU_DESCRIPTOR_HANDLE srcHandle, SRV_REGISTER reg)
{
	D3D12_CPU_DESCRIPTOR_HANDLE destHandle = GetCpuHandle(reg);

	uint32 destRange = 1;
	uint32 srcRange = 1;
	DEVICE->CopyDescriptors(1, &destHandle, &destRange, 1, &srcHandle, &srcRange, D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TableDescriptorHeap::CommitTable()
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = _descHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;

	// GPU Registers가 사용할 Table로 올림
	CMD_LIST->SetGraphicsRootDescriptorTable(0, handle);

	_currentGroupIndex++;
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCpuHandle(CBV_REGISTER reg) const
{
	return GetCpuHandle(static_cast<uint8>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCpuHandle(SRV_REGISTER reg) const
{
	return GetCpuHandle(static_cast<uint8>(reg));
}

D3D12_CPU_DESCRIPTOR_HANDLE TableDescriptorHeap::GetCpuHandle(uint8 reg) const
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = _descHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += _currentGroupIndex * _groupSize;
	handle.ptr += reg * _handleSize;

	return handle;
}
