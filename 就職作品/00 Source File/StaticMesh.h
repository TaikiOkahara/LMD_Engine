#pragma once
//#include "SkeletalMesh.h"
//#include <vector>
//#include <map>

//#include "director.h"
//#include "renderer.h"

#include "../05 Resource File/assimp/cimport.h"
#include "../05 Resource File/assimp/scene.h"
#include "../05 Resource File/assimp/postprocess.h"
#include "../05 Resource File/assimp/matrix4x4.h"
#pragma comment (lib,"assimp.lib")
//#pragma comment (lib,"assimp.lib")

struct DEFORM_VERTEX {
	aiVector3D Posistion;
	aiVector3D Normal;
	aiVector3D Tangent;
};

class StaticMesh
{
	
private:
	const aiScene* m_AiScene = nullptr;
	ID3D11Buffer** m_VertexBuffer = nullptr;
	ID3D11Buffer** m_IndexBuffer = nullptr;

	std::map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;//変形後頂点データ

public:
	void LoadModel(const char* FileName);
	void LoadTexture(std::string file_name);//Textureファイルパス

	void Unload();
	void Update();
	void Draw();
	void DrawInstanced(UINT instanceCount);

	StaticMesh(){}
	~StaticMesh(){}
};