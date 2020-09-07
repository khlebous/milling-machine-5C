#pragma once
#include "Scene/Scene.h"
#include "Graphics/Components/Transform.h"
#include "Graphics/Components/Light.h"
#include "Graphics/Components/MeshRenderer.h"
#include "IGuiWindow.h"

namespace fe
{
	class InspectorGui : public IGuiWindow
	{
	public:
		InspectorGui();

		void Render() override;
		void SetGameObject(GameObject* go) { selectedGO = go; }
		GameObject* GetSelectedGO() { return selectedGO; }

	protected:
		GameObject* selectedGO;

		void RenderGameObject(GameObject* go);
		virtual void RenderComponent(GameObject* go, Component* component);

		void RenderComponent(Transform& transform);
		void RenderComponent(MeshRenderer& meshRenderer);
		void RenderComponent(GameObject* go, Light& light);
	};
}
