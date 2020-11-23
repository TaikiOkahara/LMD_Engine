#pragma once


#pragma warning(disable : 4819)
#include "../05 Resource File/assimp/cimport.h"
#include "../05 Resource File/assimp/scene.h"
#include "../05 Resource File/assimp/postprocess.h"
#include "../05 Resource File/assimp/matrix4x4.h"
#pragma comment (lib,"assimp.lib")

#include "renderer.h"


struct DEFORM_ANIMVERTEX
{
	aiVector3D Posistion;
	aiVector3D Normal;
	aiVector3D Tangent;
	aiVector3D BiNormal;

	unsigned int BoneIndex[4];
	unsigned int BoneNum;
	std::string  BoneName[4];//本来はボーンインデックスで管理するべき
	float		 BoneWeight[4];

};

class CAnimationModel
{

	

	//ボーン構造体
	struct BONE
	{
		aiMatrix4x4 Matrix;
		aiMatrix4x4 AnimationMatrix;
		aiMatrix4x4 OffsetMatrix;
	};


private:
	const aiScene* m_pAiScene = nullptr;
	ID3D11Buffer** m_pVertexBuffer = nullptr;
	ID3D11Buffer** m_pIndexBuffer = nullptr;

	std::map<std::string, ID3D11ShaderResourceView*> m_mapTexture;
	std::map < std::string, const aiScene*> m_mapAnimation;

	std::vector<DEFORM_ANIMVERTEX>* m_vectorDeformVertex;//変形後頂点データ
	std::map < const std::string, BONE> m_mapBone;//ボーンデータ（名前で参照）
	std::string m_sCurAnimationName;

	ANIMATIONMATRIX* m_AnimationMatrix = nullptr;


	float m_fBlendTime = 1.0f;


	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);
public:
	int m_iFrame = 0;

	void LoadModel(const char* FileName,D3DXVECTOR3 pos);
	void LoadTexture(std::string file_name);
	void LoadAnimation(const char* FileName, const char* AnimationName);

	void Unload();
	void Update(const char* AnimationName,int Frame);
	void Draw();
	void DrawInstance();

};