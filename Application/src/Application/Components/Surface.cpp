#include "Surface.h"

#include <Graphics/Components/MeshRenderer.h>

CLASS_DEFINITION(fe::Component, Surface)

Surface::Surface()
{
	int size = 4;
	poles = TColgp_Array2OfPnt(0, size - 1, 0, size - 1);

	float scale = 25;
	for (int i = 0; i < size; i++)
	{
		std::vector<sm::Vector3> vector;
		points.push_back(vector);
		for (int j = 0; j < size; j++)
		{
			if (true)
			{
				poles(i, j).SetCoord(-i * scale + 50, 10, j * scale - 10);
				points[i].push_back(sm::Vector3(-i * scale + 50, 10, j * scale - 10));
			}
			else
			{
				poles(i, j).SetCoord(-i * scale + 50, sin(i + j) * scale + 10, j * scale - 10);
				points[i].push_back(sm::Vector3(-i * scale + 50, sin(i + j) * scale + 10, j * scale - 10));
			}
		}
	}

}

std::vector<fe::VertexP> Surface::GetPoints()
{
	if (BEZ) delete BEZ;
	BEZ = new Geom_BezierSurface(poles);
	std::vector<fe::VertexP> points;
	for (float i = 0; i < 1; i += 0.01)
		for (float j = 0; j < 1; j += 0.01)
		{
			gp_Pnt x;
			BEZ->D0(i, j, x);
			float xx = x.X();
			points.emplace_back(x.X(), x.Y(), x.Z());
		}

	return points;
}

void Surface::GetUI()
{
	if (ImGui::TreeNodeEx("Surface", ImGuiTreeNodeFlags_DefaultOpen))
	{
		int size = 4;
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (ImGui::DragFloat3(("point" + std::to_string(i) + std::to_string(j)).c_str(), &points[i][j].x))
				{
					sm::Vector3& v = points[i][j];
					poles(i, j).SetCoord(v.x, v.y, v.z);
					auto points = GetPoints();

					std::vector<DWORD> indices(points.size());
					std::iota(std::begin(indices), std::end(indices), 0);

					fe::MeshRenderer& mr = GetOwner().GetComponent<fe::MeshRenderer>();
					fe::IMesh* aaa = mr.GetMesh(0).mesh;
					aaa->Init(points.data(), sizeof(fe::VertexP), points.size(), indices);
				}
			}

			ImGui::Spacing();
		}
		ImGui::TreePop();
	}
}
