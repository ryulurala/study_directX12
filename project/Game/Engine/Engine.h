﻿#pragma once

class Device;
class CommandQueue;
class SwapChain;
class RootSignature;
class ConstantBuffer;
class TableDescriptorHeap;
class DepthStencilBuffer;

class Input;
class Timer;

class Engine
{
public:
	void Init(const WindowInfo& window);
	void ResizeWindow(int32 width, int32 height);

	void Render();		// 렌더링

public:
	void Update();

public:
	shared_ptr<Device> GetDevice() const { return _device; }
	shared_ptr<CommandQueue> GetCmdQueue() const { return _cmdQueue; }
	shared_ptr<SwapChain> GetSwapChain() const { return _swapChain; }
	shared_ptr<RootSignature> GetRootSignature() const {return _rootSignature;}
	shared_ptr<ConstantBuffer> GetConstantBuffer() const {return _constantBuffer;}
	shared_ptr<TableDescriptorHeap> GetTableDescHeap() const { return _tableDescHeap; }
	shared_ptr<DepthStencilBuffer> GetDepthStencilBuffer() const { return _depthStencilBuffer; }

	shared_ptr<Input> GetInput() const { return _input; }
	shared_ptr<Timer> GetTimer() const { return _timer; }

public:
	void RenderBegin();		// Command Queue에 요청 사항 넣기
	void RenderEnd();		// 쌓은 일감을 GPU에게 넘김

private:
	void ShowFps();

private:
	// 그려질 화면 크기 관련
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};

	shared_ptr<Device> _device;
	shared_ptr<CommandQueue> _cmdQueue;
	shared_ptr<SwapChain> _swapChain;
	shared_ptr<RootSignature> _rootSignature;
	shared_ptr<ConstantBuffer> _constantBuffer;
	shared_ptr<TableDescriptorHeap> _tableDescHeap;
	shared_ptr<DepthStencilBuffer> _depthStencilBuffer;

	shared_ptr<Input> _input;
	shared_ptr<Timer> _timer;
};

