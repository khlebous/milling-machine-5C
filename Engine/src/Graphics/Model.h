#pragma once
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"

namespace fe
{
	namespace sm = DirectX::SimpleMath;

	class Model
	{
	public:
		Model() { }
		Model(Model* model);
		~Model();
		bool Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, bool instance);

		size_t GetMeshesSize() { return meshes.size(); }
		IMesh* GetMesh(size_t idx) { return meshes[idx]; }
		Material* GetMaterial(size_t idx) { return materials[idx]; }
		void SetMaterial(size_t idx, Material* material) { materials[idx] = material; }

	private:
		std::vector<IMesh*> meshes;
		std::vector<Material*> materials;

		bool LoadModel(const std::string& filePath, bool instance);
		void ProcessNode(aiNode* node, const aiScene* scene, bool instance);
		IMesh* ProcessMesh(aiMesh* mesh, bool instance);
		Material* ProcessMaterial(aiMesh* mesh, const aiScene* scene);
		std::vector<Texture> LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene);

		ID3D11Device* device = nullptr;
		ID3D11DeviceContext* deviceContext = nullptr;
	};
}
