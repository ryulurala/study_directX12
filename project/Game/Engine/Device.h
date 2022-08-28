#pragma once


class Device
{
public:
	void Init();
	
	ComPtr<IDXGIFactory> GetDXGI() const { return _dxgi; }
	ComPtr<ID3D12Device> GetDevice() const { return _device; }

private:
	// COM(Component Object Model)
	// - DX의 프로그래밍 언어 독립성과 하위 호환성을 가능하게 하는 기술
	// - COM 객체(COM 인터페이스)를 사용하고, 세부사항은 사용자에게 숨겨져 있다.
	// - ComPtr: Com 객체의 스마트 포인터

	ComPtr<ID3D12Debug>		_debugController;

	// DXGI(DirectX Graphic Infrastructure)
	// - 전체 화면 모드 전환
	// - 지원되는 디스플레이 모드 열거
	ComPtr<IDXGIFactory>	_dxgi;			// 화면 관련 기능

	ComPtr<ID3D12Device>	_device;		// 각종 객체 생성

};

