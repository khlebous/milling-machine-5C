cbuffer CB_mvpMatrices : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix invViewMatrix;
	matrix projMatrix;
};

struct VS_INPUT
{
	float3 pos : POSITION;
};

struct VS_OUTPUT
{
	float4 pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT i)
{
	VS_OUTPUT o;

	float3 worldPos = mul(worldMatrix, float4(i.pos, 1.0f)).xyz;
	o.pos = mul(viewMatrix, float4(worldPos, 1.0f));
	o.pos = mul(projMatrix, o.pos);

	return o;
}
