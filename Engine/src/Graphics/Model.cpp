#include "pch.h"
#include "Model.h"
#include <assimp/pbrmaterial.h>
#include "InstancedMesh.h"

namespace fe
{
	Model::Model(Model* model)
	{
		this->device = model->device;
		this->deviceContext = model->deviceContext;

		this->meshes.reserve(model->meshes.size());
		for (int i = 0; i < model->meshes.size(); i++)
			this->meshes.push_back(model->meshes[i]);

		this->materials.reserve(model->materials.size());
		for (int i = 0; i < model->materials.size(); i++)
			this->materials.push_back(model->materials[i]);
	}

	Model::~Model()
	{
		for (size_t i = 0; i < meshes.size(); i++)
			delete meshes[i];

		for (size_t i = 0; i < materials.size(); i++)
			delete materials[i];
	}

	bool Model::Initialize(const std::string& filePath, ID3D11Device* device, ID3D11DeviceContext* deviceContext, bool instance)
	{
		this->device = device;
		this->deviceContext = deviceContext;

		try
		{
			if (!this->LoadModel(filePath, instance))
				return false;
		}
		catch (COMException& exception)
		{
			ErrorLogger::Log(exception);
			return false;
		}

		return true;
	}

	bool Model::LoadModel(const std::string& filePath, bool instance)
	{
		Assimp::Importer importer;

		const aiScene* pScene = importer.ReadFile(filePath,
			aiProcess_Triangulate |
			aiProcess_ConvertToLeftHanded);

		if (pScene == nullptr)
			return false;

		this->ProcessNode(pScene->mRootNode, pScene, instance);
		return true;
	}

	void Model::ProcessNode(aiNode* node, const aiScene* scene, bool instance)
	{
		for (UINT i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(this->ProcessMesh(mesh, instance));
			materials.push_back(this->ProcessMaterial(mesh, scene));
		}

		for (UINT i = 0; i < node->mNumChildren; i++)
		{
			this->ProcessNode(node->mChildren[i], scene, instance);
		}
	}

	IMesh* Model::ProcessMesh(aiMesh* mesh, bool instance)
	{
		// Data to fill
		std::vector<VertexPN> vertices;
		std::vector<DWORD> indices;

		//Get vertices
		for (UINT i = 0; i < mesh->mNumVertices; i++)
		{
			VertexPN vertex;

			vertex.pos.x = mesh->mVertices[i].x;
			vertex.pos.y = mesh->mVertices[i].y;
			vertex.pos.z = mesh->mVertices[i].z;

			vertex.normal.x = mesh->mNormals[i].x;
			vertex.normal.y = mesh->mNormals[i].y;
			vertex.normal.z = mesh->mNormals[i].z;

			/*if (mesh->mTextureCoords[0])
			{
				vertex.tex.x = (float)mesh->mTextureCoords[0][i].x;
				vertex.tex.y = (float)mesh->mTextureCoords[0][i].y;
			}*/

			vertices.push_back(vertex);
		}

		//Get indices
		for (UINT i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];

			for (UINT j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		if (instance)
		{
			return new InstancedMesh(this->device, this->deviceContext, vertices.data(), sizeof(VertexPN), vertices.size(), indices);
		}
		return new Mesh(this->device, this->deviceContext, vertices.data(), sizeof(VertexPN), vertices.size(), indices);
	}

	Material* Model::ProcessMaterial(aiMesh* mesh, const aiScene* scene)
	{
		Material* material = new Material();
		if (scene->mNumMaterials <= 1) // From 1 because default material is also read
			return material;

		aiMaterial* pMaterial = scene->mMaterials[mesh->mMaterialIndex];
		aiReturn aiReturn;
		aiString aiName;
		aiColor3D aiColor;
		ai_real aiValue;

		aiReturn = pMaterial->Get(AI_MATKEY_NAME, aiName);
		if (aiReturn == aiReturn_SUCCESS)
		{
			material->name = aiName.C_Str();
		}

		return material;
	}

	std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* pMaterial, aiTextureType textureType, const aiScene* pScene)
	{
		std::vector<Texture> materialTextures;
		TextureStorageType storetype = TextureStorageType::Invalid;
		unsigned int textureCount = pMaterial->GetTextureCount(textureType);

		if (textureCount == 0) //If there are no textures
		{
			storetype = TextureStorageType::None;
			aiColor3D aiColor(0.0f, 0.0f, 0.0f);
			switch (textureType)
			{
			case aiTextureType_DIFFUSE:
				pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);
				auto desc = Texture::GetOneColorTexDesc();
				auto color = sm::Color(aiColor.r, aiColor.g, aiColor.b);
				materialTextures.push_back(Texture(this->device, desc, &color, textureType));
				return materialTextures;
			}
		}
		else
		{
			auto desc = Texture::GetOneColorTexDesc();
			auto color = sm::Color(0, 1, 0, 1);
			materialTextures.push_back(Texture(this->device, desc, &color, aiTextureType::aiTextureType_DIFFUSE));

			return materialTextures;
		}
	}
}
