#define MAX_POINT_LIGHTS 5

struct PointLight
{
	float4 position;
	float4 ambient;
	float4 diffuse;
	float4 specular;
};

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

cbuffer CB_lightsCount : register(b0)
{
	int pointLightCount;
};

cbuffer CB_pointLight : register(b1)
{
	PointLight pointLight[MAX_POINT_LIGHTS];
};

cbuffer CB_material : register(b2)
{
	Material material;
};

struct PS_INPUT
{
	float4 pos    : SV_POSITION;
	float3 worldPos    : POSITION;
	float3 norm : NORMAL;
	float3 viewVec     : POSITION1;
};

float3 GetPointLightColor(const PointLight light, float3 normal, float3 fragPos, float3 viewDir);

float4 main(PS_INPUT i) : SV_TARGET
{
	float3 viewVec = normalize(i.viewVec);
	float3 normal = normalize(i.norm);
	float3 worldPos = i.worldPos;

	float3 color = float3(0, 0, 0);

	for (int i = 0; i < pointLightCount; i++)
	{
		color += GetPointLightColor(pointLight[i], normal, worldPos, viewVec);
	}

	return float4(color, 1.0);
}

float3 GetPointLightColor(const PointLight light, float3 normal, float3 fragPos, float3 viewDir)
{
	float3 lightDir = normalize(light.position - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	float3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.m);
	// combine results
	float3 ambient = light.ambient;
	float3 diffuse = light.diffuse * diff;
	float3 specular = light.specular * spec;

	return (ambient * material.ambient + diffuse * material.diffuse + specular * material.specular);
}