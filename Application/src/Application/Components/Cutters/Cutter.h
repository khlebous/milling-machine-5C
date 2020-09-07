#pragma once

#include <Graphics/Components/Component.h>
#include <Graphics/Vertex.h>

#include <ImGui/imgui.h>
#include <vector>

//enum CutterType
//{
//	Flat = 0,
//	Spherical = 1,
//	Toroidal = 2,
//};

class Cutter : public fe::Component
{
	CLASS_DECLARATION(Cutter)

public:
	Cutter(float radius, float startHeight);

	float cutRadius;
	std::vector<fe::VertexPN> vertices;
	std::vector<DWORD> indices;

	void GetUI() final;

protected:
	void InitUpperPart(int horizontalLvls, float startHeight, float height);
	virtual void InitBottomPart(int horizontalLvls, int roundLvls) { }
};
