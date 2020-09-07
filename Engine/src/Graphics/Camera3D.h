#pragma once
#include "GameObjects/GameObject.h"
#include "Components/Transform.h"

namespace fe
{
	namespace sm = DirectX::SimpleMath;

	class Camera3D
	{
	public:
		Camera3D();

		void SetPosition(float x, float y, float z);
		void SetPosition(sm::Vector3 pos);
		void SetRotation(float x, float y, float z);
		void SetProjectionValues(float fovDegrees, float width, float height, float nearZ, float farZ);
		void SetWidthHeight(float width, float height);

		const Transform& GetTransform() const { return this->transform; }
		const DirectX::SimpleMath::Matrix& GetViewMatrix() const { return this->viewMatrix; }
		const DirectX::SimpleMath::Matrix& GetProjectionMatrix() const { return this->projectionMatrix; }

		void AdjustRotation(float x, float y, float z);
		void AdjustPosition(sm::Vector3 pos);
		void AdjustPosition(float x, float y, float z);

		static float pixelWidth;
		static float pixelHeight;

	private:
		void UpdateProjectionMatrix();
		void UpdateViewMatrix();

		sm::Matrix viewMatrix;
		sm::Matrix projectionMatrix;

		Transform transform;

		float fov;
		float aspectRatio;
		float nearZ, farZ;
	};
}
