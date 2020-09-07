#include "pch.h"
#include "Light.h"

namespace fe
{
	CLASS_DEFINITION(Component, Light)

		Light::Light(LightType type) : Light(type, sm::Color(), sm::Color(), sm::Color()) { }

	Light::Light(LightType type, sm::Color ambient, sm::Color diffuse, sm::Color specular) :
		Component()
	{
		this->type = type;

		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular;
	}

	Light& Light::operator=(const Light& light)
	{
		this->type = light.type;

		this->ambient = light.ambient;
		this->diffuse = light.diffuse;
		this->specular = light.specular;

		return *this;
	}
}
