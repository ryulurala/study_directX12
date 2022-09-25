#pragma once
class RootSignature
{
public:
	void Init();

	ComPtr<ID3D12RootSignature> GetSignature() const { return _signature; }

private:
	void CreateSamplerDesc();
	void CreateRootSignature();

private:
	ComPtr<ID3D12RootSignature> _signature;
	D3D12_STATIC_SAMPLER_DESC	_samplerDesc;

};

