#include "pch.h"
#include "ConstantBuffer.h"

#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"

ConstantBuffer::ConstantBuffer()
{

}

ConstantBuffer::~ConstantBuffer()
{
	if (_cbvBuffer)
	{
		_cbvBuffer->Unmap(0, nullptr);
		_cbvBuffer = nullptr;
	}
}

void ConstantBuffer::Init(uint32 size, uint32 count)
{
	// 상수 버퍼는 256 byte 배수로 만들어야 함.
	// 0 256 512 768
	// 1 -> 256, 257 -> 512
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	CreateBuffer();
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0;
}

void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	assert(_currentIndex < _elementSize);

	// Buffer에 데이터 복제
	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	// Buffer의 주소를 Register에 전송
	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);
	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address);
	_currentIndex++;
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize;

	return objCBAddress;
}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer)
	);

	// Resource가 끝날 때까지 Un-map을 할 필요가 없다.
	// 대신, GPU에서 사용하는 Resource는 쓰지 않아야 한다.
	// for. 동기화 필요 like WaitSync()
	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
}
