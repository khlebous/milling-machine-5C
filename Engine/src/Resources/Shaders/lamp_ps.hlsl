struct PS_INPUT
{
	float4 pos : SV_POSITION; 
};

float4 main(PS_INPUT i) : SV_TARGET
{
	return float4(1.0, 1.0f, 1.0f, 1.0f);
}
