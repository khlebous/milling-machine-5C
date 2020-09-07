#pragma once
#include "Transform.h"
#include "Graphics/Enums/LightType.h"

namespace fe
{
	class InspectorGui;
	namespace sm = DirectX::SimpleMath;

	class Light : public Component
	{
		CLASS_DECLARATION(Light)

	public:
		Light() : Light(LightType::Point) { }
		Light(LightType type);
		Light(LightType type, sm::Color ambient, sm::Color diffuse, sm::Color specular);
		Light& operator=(const Light& light);

		const LightType& GetType() const { return type; }
		const sm::Color& GetAmbient() const { return ambient; }
		const sm::Color& GetDiffuse() const { return diffuse; }
		const sm::Color& GetSpecular() const { return specular; }

		void SetType(const LightType& type) { this->type = type; }
		void SetAmbient(const sm::Color& ambient) { this->ambient = ambient; }
		void SetDiffuse(const sm::Color& diffuse) { this->diffuse = diffuse; }
		void SetSpecular(const sm::Color& specular) { this->specular = specular; }

		friend class InspectorGui;

	private:
		LightType type;

		sm::Color ambient;
		sm::Color diffuse;
		sm::Color specular;
	};
}
