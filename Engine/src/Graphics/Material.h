#pragma once

namespace fe
{
	namespace sm = DirectX::SimpleMath;

	class Material
	{
	public:
		Material() : Material(sm::Color(0.3f, 0.3f, 0.3f), sm::Color(0.3f, 0.3f, 0.3f),
			sm::Color(0.3f, 0.3f, 0.3f), 1) { }
		Material(const sm::Color& ambient, const sm::Color& diffuse,
			const sm::Color& specular, float specularExponent) :
			Material(ambient, diffuse, specular, specularExponent, "material" + std::to_string(count++)) { }
		Material(const sm::Color& ambient, const sm::Color& diffuse, const sm::Color& specular,
			float specularExponent, std::string name)
		{
			this->ambient = ambient;
			this->diffuse = diffuse;
			this->specular = specular;
			this->specularExponent = specularExponent;

			this->name = name;
		}
		Material(const Material& m) : Material(m.ambient, m.diffuse, m.specular, m.specularExponent, m.name) {    };
		Material& operator=(const Material& m)
		{
			this->ambient = m.ambient;
			this->diffuse = m.diffuse;
			this->specular = m.specular;
			this->specularExponent = m.specularExponent;

			name = m.name;

			return *this;
		}

		sm::Color ambient;
		sm::Color diffuse;
		sm::Color specular;
		float specularExponent; // ranges between 0 and 1000

		std::string name;

	private:
		static int count;
	};
}
