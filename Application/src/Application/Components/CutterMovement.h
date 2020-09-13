#pragma once

#include <Graphics/Components/Component.h>
#include <Graphics/InstancedMesh.h>
#include <Graphics/GameObjects/GameObject.h>

#include "Surface.h"

class Cutter;

class CutterMovement : public fe::Component
{
	CLASS_DECLARATION(CutterMovement)

public:
	CutterMovement(Surface* surface);

	Surface* surface;
	fe::InstancedMesh* instancedMesh;
	float u, v;

	void OnUpdate() final;
};
