#include "pch.h"
#include "Mesh.h"

#include "Engine.h"
#include "Device.h"
#include "CommandQueue.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "Texture.h"

void Mesh::Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer)
{
	CreateVertexBuffer(vertexBuffer);
	CreateIndexBuffer(indexBuffer);
}

void Mesh::Render()
{
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView);	// Slot: (0~15)
	CMD_LIST->IASetIndexBuffer(&_indexBufferView);

	// 1) Buffer에 Data 설정
	D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetConstantBuffer()->PushData(0, &_transform, sizeof(_transform));
		
	// 2) Table Descriptor Heap에 CBV 전달
	GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b0);

	// Table Descriptor Heap에 SRV 전달
	GEngine->GetTableDescHeap()->SetSRV(_tex->GetCpuHandle(), SRV_REGISTER::t0);

	// 3) Table Descriptor Heap 커밋
	// GPU Registers가 사용할 수 있도록 올리는 작업
	GEngine->GetTableDescHeap()->CommitTable();

	//CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);		// Vertex 이용
	CMD_LIST->DrawIndexedInstanced(_indexCount, 1, 0, 0, 0);
}

void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer)
{
	_vertexCount = static_cast<uint32>(buffer.size());		// for. Render
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
	::memcpy(vertexDataBuffer, &buffer[0], bufferSize);
	_vertexBuffer->Unmap(0, nullptr);

	// Vertex Buffer View 초기화
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex);	// 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize;			// 버퍼의 크기
}

void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	_indexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _indexCount * sizeof(uint32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_indexBuffer)
	);

	void* IndexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0);
	_indexBuffer->Map(0, &readRange, &IndexDataBuffer);
	::memcpy(IndexDataBuffer, &buffer[0], bufferSize);
	_indexBuffer->Unmap(0, nullptr);
	
	_indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
	_indexBufferView.Format = DXGI_FORMAT_R32_UINT;		// 32 bits
	_indexBufferView.SizeInBytes = bufferSize;
}
