#include "pch.h"
#include "InspectorGui.h"
#include "ImGui/imgui.h"
#include "Utils/ImGuiUtils.h"
#include <numeric>

namespace fe
{
	InspectorGui::InspectorGui()
		:
		IGuiWindow("Inspector", true),
		selectedGO(nullptr)
	{ }

	void InspectorGui::Render()
	{
		if (open)
		{
			ImGui::Begin("Inspector", &open);

			if (selectedGO != nullptr)
				RenderGameObject(selectedGO);

			ImGui::End();
		}
	}

	void InspectorGui::RenderGameObject(GameObject* go)
	{
		ImGui::InputText("##go_name", &go->name[0], 24);
		ImGui::SameLine();
		ImGui::Checkbox("##go_active", &go->active);

		for (auto& c : go->components)
		{
			RenderComponent(go, c.get());
			c.get()->GetUI();
		}
	}

	void InspectorGui::RenderComponent(GameObject* go, Component* component)
	{
		if (component->IsClassType(Transform::Type))
		{
			Transform& transform = *(static_cast<Transform*>(component));
			RenderComponent(transform);
		}

		if (component->IsClassType(Light::Type))
		{
			Light& light = *(static_cast<Light*>(component));
			RenderComponent(go, light);
		}

		if (component->IsClassType(MeshRenderer::Type))
		{
			MeshRenderer& meshRenderer = *(static_cast<MeshRenderer*>(component));
			RenderComponent(meshRenderer);
		}
	}

	void InspectorGui::RenderComponent(Transform& transform)
	{
		if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::DragFloat3("position", &transform.position.x, 0.1f);
			ImGui::DragFloat3("rotation", &transform.rotation.x, 0.1f);
			ImGui::DragFloat3("scale", &transform.scale.x, 0.1f);

			ImGui::TreePop();
		}
	}

	void InspectorGui::RenderComponent(GameObject* go, Light& light)
	{
		if (ImGui::TreeNodeEx("Light", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Point light");
			ImGui::ColorEdit3("Light ambient", &light.ambient.x);
			ImGui::ColorEdit3("Light diffuse", &light.diffuse.x);
			ImGui::ColorEdit3("Light specular", &light.specular.x);

			ImGui::TreePop();
		}
	}

	void InspectorGui::RenderComponent(MeshRenderer& meshRenderer)
	{
		if (ImGui::TreeNodeEx("ModelRenderer", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Some model");

			for (size_t mesh_idx = 0; mesh_idx < meshRenderer.GetMeshesSize(); mesh_idx++)
			{
				if (ImGui::TreeNodeEx(("Mesh" + std::to_string(mesh_idx)).c_str(), ImGuiTreeNodeFlags_DefaultOpen))
				{
					RenderableMesh& rm = meshRenderer.GetMesh(mesh_idx);

					IMesh* mesh = rm.mesh;
					ImGui::Text(("mesh: " + std::to_string(mesh->VerticesCount()) +
						" vertices, " + std::to_string(mesh->TrianglesCount()) + " triangles").c_str());

					ImGui::Spacing();

					Material* material = rm.material;
					if (material != nullptr)
					{
						ImGui::Text(("material: " + material->name).c_str());
						ImGui::ColorEdit3("ambient", &material->ambient.x);
						ImGui::ColorEdit3("diffuse", &material->diffuse.x);
						ImGui::ColorEdit3("specular", &material->specular.x);
						ImGui::SliderFloat("specularExponent", &material->specularExponent, 0.0f, 1000.0f);
					}

					ImGui::TreePop();
				}
			}

			ImGui::TreePop();
		}
	}
}
