struct Material
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float m;


	float _not_used0;
	float _not_used1;
	float _not_used2;
};

cbuffer CB_material : register(b2)
{
	Material material;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION; 
};

float4 main(PS_INPUT i) : SV_TARGET
{
	return float4(material.ambient.xyz, 1.0);
}
