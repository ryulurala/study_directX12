struct VS_IN
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct VS_OUT
{
	float4 pos : SV_Position;
	float4 color : COLOR;
};

// Vertex Shader에 사용될 함수
VS_OUT VS_Main(VS_IN input)		
{
	VS_OUT output = (VS_OUT)0;

	output.pos = float4(input.pos, 1.f);
	output.color = input.color;

	return output;
}

// Pixel Shader에 사용될 함수
float4 PS_Main(VS_OUT input) : SV_Target
{
	return input.color;
}