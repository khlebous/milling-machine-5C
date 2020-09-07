#include "pch.h"
#include "HierarchyGui.h"
#include "ImGui/imgui.h"

namespace fe
{
	HierarchyGui::HierarchyGui()
		:
		IGuiWindow("Hierarchy", true),
		scene(nullptr),
		inspector(nullptr)
	{ }

	void HierarchyGui::Initialize(Scene* scene, InspectorGui* inspector)
	{
		this->scene = scene;
		this->inspector = inspector;

		size_t goSize = scene->gameObjects.size();
		if (goSize > 0)
			this->inspector->SetGameObject(scene->gameObjects[goSize - 1]);
	}

	void HierarchyGui::Render()
	{
		if (open)
		{
			ImGui::Begin("Hierarchy", &open);

			int idx = 0;
			for (auto& go : scene->gameObjects)
				RenderGameObject(go, idx);

			ImGui::End();
		}
	}

	void HierarchyGui::RenderGameObject(GameObject* go, int& idx)
	{
		idx++;

		int node_clicked = -1;
		ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_DefaultOpen | (inspector->GetSelectedGO() == go ? ImGuiTreeNodeFlags_Selected : 0);

		if (go->children.size() > 0)
		{
			bool node_open = ImGui::TreeNodeEx((void*)(intptr_t)idx, node_flags, go->name.c_str(), idx);
			if (ImGui::IsItemClicked())
				node_clicked = idx;
			if (node_open)
			{
				for (auto& goChild : go->children)
					RenderGameObject(goChild, idx);

				ImGui::TreePop();
			}
		}
		else
		{
			node_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
			ImGui::TreeNodeEx((void*)(intptr_t)idx, node_flags, go->name.c_str(), idx);
			if (ImGui::IsItemClicked())
				node_clicked = idx;
		}

		if (node_clicked != -1)
			inspector->SetGameObject(go);
	}
}
