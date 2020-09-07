#pragma once
#include "Scene/Scene.h"
#include "InspectorGui.h"
#include "IGuiWindow.h"

namespace fe
{
	class HierarchyGui : public IGuiWindow
	{
	public:
		HierarchyGui();

		void Initialize(Scene* scene, InspectorGui* inspector);
		void Render() override;

	protected:
		Scene* scene;
		InspectorGui* inspector;

		void RenderGameObject(GameObject* go, int& i);
	};
}
