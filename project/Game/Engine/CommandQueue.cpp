#include "pch.h"
#include "CommandQueue.h"

#include "SwapChain.h"
#include "Engine.h"
#include "RootSignature.h"
#include "Engine.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"

CommandQueue::~CommandQueue()
{
	// Event 해제
	::CloseHandle(_fenceEvent);
}

void CommandQueue::Init(ComPtr<ID3D12Device> device, shared_ptr<SwapChain> swapChain)
{
	_swapChain = swapChain;
	
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;

	device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&_cmdQueue));
	
	// D3D12_COMMAND_LIST_TYPE_DIRECT: GPU가 직접 실행하는 명령 목록
	device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&_cmdAlloc));

	// GPU가 하나인 시스템에서는 0으로
	// DIRECT or BUNDLE
	// Allocator
	// 초기 상태(그리기 명령은 nullptr 지정)
	device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, _cmdAlloc.Get(), nullptr, IID_PPV_ARGS(&_cmdList));
	
	// CommandList는 Open / Close 상태가 있고,
	// Open 상태에서 Command를 넣다가 Close한 다음 제출하는 개념
	_cmdList->Close();
	
	// CreateFence
	// - CPU와 GPU의 동기화 수단으로 쓰인다
	device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&_fence));
	_fenceEvent = ::CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void CommandQueue::WaitSync()
{
	// 일감 상태 번호를 표시
	_fenceValue++;
	
	// Command Queue에도 펜스 번호를 알려준다.
	// GPU가 "Signal()" 이전 명령들을 처리 완료 전까지 새 울타리가 설정되지 않는다.
	_cmdQueue->Signal(_fence.Get(), _fenceValue);
	
	// GPU가 해당 펜스 지점까지 명령을 완료할 때까지 CPU는 기다린다.
	if (_fence->GetCompletedValue() < _fenceValue)
	{
		// GPU가 현재 펜스에 도달하면 Event가 발생한다.
		_fence->SetEventOnCompletion(_fenceValue, _fenceEvent);
		
		// GPU가 현재 펜스에서 발생할 Event에 도달할 때까지 CPU는 기다린다.
		::WaitForSingleObject(_fenceEvent, INFINITE);	// CPU 계속 기다림.
	}
}

void CommandQueue::RenderBegin(const D3D12_VIEWPORT* vp, const D3D12_RECT* rect)
{
	// 이전 내용 clear
	_cmdAlloc->Reset();
	_cmdList->Reset(_cmdAlloc.Get(), nullptr);

	// Back Buffer Resource를 "화면 출력용"에서 "외주 결과물"로 변경 예약
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),	// Back Buffer Resource
		D3D12_RESOURCE_STATE_PRESENT,			// before. 화면 출력
		D3D12_RESOURCE_STATE_RENDER_TARGET		// after. 외주 결과물
	);

	_cmdList->SetGraphicsRootSignature(ROOT_SIGNATURE.Get());	// 서명
	GEngine->GetConstantBuffer()->Clear();
	GEngine->GetTableDescHeap()->Clear();

	ID3D12DescriptorHeap* descHeap = GEngine->GetTableDescHeap()->GetDescriptorHeap().Get();
	_cmdList->SetDescriptorHeaps(1, &descHeap);		// 사용할 Descriptor Heap 지정: 무거움

	// 명령 목록에 "변경 예약"을 삽입
	_cmdList->ResourceBarrier(1, &barrier);

	// viewport와 scissor rect를 설정한다.
	// Command List가 리셋될 때마다 설정해야 한다.
	_cmdList->RSSetViewports(1, vp);
	_cmdList->RSSetScissorRects(1, rect);
	
	// 렌더할 버퍼 구체화.
	// GPU에게 결과물을 계산해달라고 요청 명령 삽입
	D3D12_CPU_DESCRIPTOR_HANDLE backBufferView = _swapChain->GetBackRTV();
	_cmdList->ClearRenderTargetView(backBufferView, Colors::LightSteelBlue, 0, nullptr);
	_cmdList->OMSetRenderTargets(1, &backBufferView, FALSE, nullptr);
}

void CommandQueue::RenderEnd()
{
	// Back Buffer Resource를 "외주 결과물"에서 "화면 출력용"으로 변경 예약
	D3D12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
		_swapChain->GetBackRTVBuffer().Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET,		// before. 외주 결과물
		D3D12_RESOURCE_STATE_PRESENT			// after. 화면 출력
	);

	// 명령 목록에 "변경 예약"을 삽입
	_cmdList->ResourceBarrier(1, &barrier);
	_cmdList->Close();

	// Command List 실행
	ID3D12CommandList* cmdListArr[] = { _cmdList.Get() };
	_cmdQueue->ExecuteCommandLists(_countof(cmdListArr), cmdListArr);	// 실질적 요청

	// 화면을 채워줌
	_swapChain->Present();

	// GPU가 모든 명령 실행할 때까지 CPU가 대기
	WaitSync();

	_swapChain->SwapIndex();
}
