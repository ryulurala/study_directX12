#include "pch.h"
#include "Mesh.h"

#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "ConstantBuffer.h"

void Mesh::Init(vector<Vertex>& vec)
{
	_vertexCount = static_cast<uint32>(vec.size());		// for. Render
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty, 
		D3D12_HEAP_FLAG_NONE, 
		&desc, 
		D3D12_RESOURCE_STATE_GENERIC_READ, 
		nullptr, 
		IID_PPV_ARGS(&_vertexBuffer)
	);

	// 삼각형 정보를 Vertex Buffer(in. GPU)로 복사
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);
	::memcpy(vertexDataBuffer, &vec[0], bufferSize);
	_vertexBuffer->Unmap(0, nullptr);

	// Vertex Buffer View 초기화
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex);	// 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize;			// 버퍼의 크기
}

void Mesh::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView);	// Slot: (0~15)

	// 밀어넣기
	GEngine->GetConstantBuffer()->PushData(0, &_transform, sizeof(_transform));
	GEngine->GetConstantBuffer()->PushData(1, &_transform, sizeof(_transform));

	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}
