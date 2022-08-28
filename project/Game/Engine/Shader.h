#pragma once

class Device;
class RootSignature;

class Shader
{
public:
	void Init(const wstring& path, ComPtr<ID3D12Device> device, ComPtr<ID3D12GraphicsCommandList> cmdList, ComPtr<ID3D12RootSignature> signature);
	void Update();

private:
	void CreateShader(const wstring& path, const string& name, const string& version, ComPtr<ID3DBlob>& blob, D3D12_SHADER_BYTECODE& shaderByteCode);
	void CreateVertexShader(const wstring& path, const string& name, const string& version);
	void CreatePixelShader(const wstring& path, const string& name, const string& version);

private:
	ComPtr<ID3DBlob>	_vsBlob;
	ComPtr<ID3DBlob>	_psBlob;
	ComPtr<ID3DBlob>	_errBlob;
	
	ComPtr<ID3D12PipelineState>			_pipelineState;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC	_pipelineDesc = {};

	ComPtr<ID3D12GraphicsCommandList>	_cmdList;
};

