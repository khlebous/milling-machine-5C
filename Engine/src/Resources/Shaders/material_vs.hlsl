cbuffer CB_mvpMatrices : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix invViewMatrix;
	matrix projMatrix;
};

struct VS_INPUT
{
	float3 pos     : POSITION;
	float3 norm  : NORMAL;
	float4 world  : WORLD;
	//row_major float4x4 world  : WORLD;
	uint InstanceId : SV_InstanceID;
};


struct VS_OUTPUT
{
	float4 pos    : SV_POSITION;
	float3 worldPos    : POSITION;
	float3 norm : NORMAL;
	float3 viewVec : POSITION1;
};

VS_OUTPUT main(VS_INPUT i)
{
	VS_OUTPUT o;

	o.worldPos = i.pos + i.world;// mul(float4(i.pos, 1.0f), i.world).xyz;
	o.pos = mul(viewMatrix, float4(o.worldPos, 1.0f));
	o.pos = mul(projMatrix, o.pos);
	
	o.norm = i.norm;// mul(float4(i.norm, 0.0f), i.world).xyz;
	
	float3 camPos = mul(invViewMatrix, float4(0.0f, 0.0f, 0.0f, 1.0f)).xyz; 
	o.viewVec = camPos - o.worldPos;
	
	return o;
}
