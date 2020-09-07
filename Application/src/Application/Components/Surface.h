#pragma once

#include <vector>
#include <numeric>

#include <Graphics/Components/Component.h>
#include <Graphics/GameObjects/GameObject.h>
#include <Graphics/Vertex.h>

#include <ImGui/imgui.h>

#include <Geom_BezierSurface.hxx>
#include <TColgp_Array2OfPnt.hxx>

namespace sm = DirectX::SimpleMath;

class Surface : public fe::Component
{
	CLASS_DECLARATION(Surface)

public:
	Surface();
	std::vector<fe::VertexP> GetPoints();
	void GetUI() override;

	Geom_BezierSurface* BEZ;
	TColgp_Array2OfPnt poles;
	std::vector<std::vector<sm::Vector3>> points;

private:
	float scale = 10;
};
