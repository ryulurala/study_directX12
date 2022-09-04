#include "pch.h"
#include "RootSignature.h"

void RootSignature::Init(ComPtr<ID3D12Device> device)
{
	// D3D12_SHADER_VISIBILITY: 특정 단계를 지정하면 다음 단계에서는 안 보이는 상태(소실)
	// D3D12_SHADER_VISIBILITY_ALL: 모든 단계에서 사용 가능
	CD3DX12_ROOT_PARAMETER param[2];
	param[0].InitAsConstantBufferView(0);	// 0 - root CBV - b0
	param[1].InitAsConstantBufferView(1);	// 1 - root CBV - b1

	D3D12_ROOT_SIGNATURE_DESC sigDesc = CD3DX12_ROOT_SIGNATURE_DESC(2, param);
	sigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;	// 입력 조립기 단계

	ComPtr<ID3DBlob> blobSignature;
	ComPtr<ID3DBlob> blobError;
	::D3D12SerializeRootSignature(&sigDesc, D3D_ROOT_SIGNATURE_VERSION_1, &blobSignature, &blobError);
	device->CreateRootSignature(0, blobSignature->GetBufferPointer(), blobSignature->GetBufferSize(), IID_PPV_ARGS(&_signature));
}
