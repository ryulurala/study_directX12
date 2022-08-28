#pragma once

class Device;
class CommandQueue;

class Mesh
{
public:
	void Init(vector<Vertex>& vec, ComPtr<ID3D12Device> device, ComPtr<ID3D12GraphicsCommandList> cmdList);
	void Render();

private:
	ComPtr<ID3D12Resource>		_vertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW	_vertexBufferView = {};
	uint32 _vertexCount = 0;

	ComPtr<ID3D12GraphicsCommandList>	_cmdList;
};

