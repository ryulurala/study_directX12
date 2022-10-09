#pragma once
class Timer
{
public:
	void Init();
	void Update();

	uint32 GetFps() const { return _fps; }
	float GetDeltaTime()const { return _deltaTime; }

private:
	uint64 _frequency = 0;
	uint64 _prevCount = 0;
	float _deltaTime = 0.f;	// 이전 프레임에서 현재 프레임까지 걸린 시간

private:
	// 프레임 계산 용도
	uint32 _frameCount = 0;
	float _frameTime = 0.f;
	uint32 _fps = 0;

};

