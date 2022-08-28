#pragma once
class RootSignature
{
public:
	void Init(ComPtr<ID3D12Device> device);

	ComPtr<ID3D12RootSignature> GetSignature() const { return _signature; }

private:
	ComPtr<ID3D12RootSignature> _signature;
};

