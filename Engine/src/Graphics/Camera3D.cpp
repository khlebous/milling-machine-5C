#include "pch.h"
#include "Camera3D.h"
#include "Utils/Vector3Ext.h"

namespace fe
{
	Camera3D::Camera3D() 
		: 
		transform(Transform())
	{
		SetPosition(0.0f, 30.0f, -15.0f);
		SetProjectionValues(90.0f, 1.0f, 1.0f, 0.1f, 1000.0f);
	}

	void Camera3D::SetPosition(float x, float y, float z)
	{
		transform.SetPosition(x, y, z);
		UpdateViewMatrix();
	}

	void Camera3D::SetPosition(sm::Vector3 pos)
	{
		transform.SetPosition(pos);
		UpdateViewMatrix();
	}

	void Camera3D::SetRotation(float x, float y, float z)
	{
		transform.SetRotation(x, y, z);
		UpdateViewMatrix();
	}

	void Camera3D::SetProjectionValues(float fovDegrees, float width, float height, float nearZ, float farZ)
	{
		this->fov = (fovDegrees / 360.0f) * DirectX::XM_2PI;
		this->pixelWidth = width;
		this->pixelHeight = height;
		this->farZ = farZ;
		this->nearZ = nearZ;
		this->aspectRatio = width / height;

		UpdateProjectionMatrix();
	}

	void Camera3D::SetWidthHeight(float width, float height)
	{
		this->pixelWidth = width;
		this->pixelHeight = height;
		this->aspectRatio = width / height;

		UpdateProjectionMatrix();
	}

	void Camera3D::AdjustRotation(float x, float y, float z)
	{
		transform.AdjustRotation(x, y, z);
		UpdateViewMatrix();
	}

	void Camera3D::AdjustPosition(sm::Vector3 pos)
	{
		transform.AdjustPosition(pos);
		UpdateViewMatrix();
	}

	void Camera3D::AdjustPosition(float x, float y, float z)
	{
		transform.AdjustPosition(x, y, z);
		UpdateViewMatrix();
	}

	void Camera3D::UpdateViewMatrix() //Updates view matrix and also updates the movement vectors
	{
		sm::Vector3 rotation = transform.GetRotation();
		sm::Vector3 position = transform.GetPosition();

		//Calculate Camera3D rotation matrix
		sm::Matrix camRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
		//Calculate unit vector of cam target based off Camera3D forward value transformed by cam rotation matrix
		sm::Vector3 camTarget = XMVector3TransformCoord(Vector3Ext::DEFAULT_FORWARD_VECTOR, camRotationMatrix);
		//Adjust cam target to be offset by the Camera3D's current position
		camTarget += position;
		//Calculate up direction based on current rotation
		sm::Vector3 upDir = XMVector3TransformCoord(Vector3Ext::DEFAULT_UP_VECTOR, camRotationMatrix);
		//Rebuild view matrix
		this->viewMatrix = XMMatrixLookAtLH(position, camTarget, upDir);
	}

	void Camera3D::UpdateProjectionMatrix()
	{
		this->projectionMatrix = DirectX::XMMatrixPerspectiveFovLH(this->fov, this->aspectRatio, this->nearZ, this->farZ);
	}

	float Camera3D::pixelWidth = 0;
	float Camera3D::pixelHeight = 0;
}
