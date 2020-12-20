#pragma once

#pragma warning(disable : 4819)

#include "../05 Resource File/assimp/cimport.h"
#include "../05 Resource File/assimp/scene.h"
#include "../05 Resource File/assimp/postprocess.h"
#include "../05 Resource File/assimp/matrix4x4.h"
#pragma comment (lib,"assimp.lib")



class StaticMesh
{
	struct DEFORM_VERTEX {
		aiVector3D Posistion;
		aiVector3D Normal;
		aiVector3D Tangent;
	};
private:
	const aiScene* m_pAiScene = nullptr;
	ID3D11Buffer** m_ppVertexBuffer = nullptr;
	ID3D11Buffer** m_ppIndexBuffer = nullptr;

	std::map<std::string, ID3D11ShaderResourceView*> m_mapTexture;

	//std::vector<DEFORM_VERTEX>* m_vectorDeformVertex;//変形後頂点データ

public:
	void LoadModel(const char* FileName);
	void LoadTexture(std::string file_name);

	void Unload();
	void Update();
	void Draw();
	void DrawInstanced(UINT instanceCount);
};