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

	int			BoneIndex[4];
	int			BoneNum;
	std::string BoneName[4];//本来はボーンインデックスで管理するべき
	float		BoneWeight[4];

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
	const aiScene* m_AiScene = nullptr;
	ID3D11Buffer** m_VertexBuffer = nullptr;
	ID3D11Buffer** m_IndexBuffer = nullptr;

	std::map<std::string, ID3D11ShaderResourceView*> m_Texture;
	std::map < std::string, const aiScene*> m_Animation;

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);

	std::vector<DEFORM_ANIMVERTEX>* m_DeformVertex;//変形後頂点データ
	std::map < const std::string, BONE> m_Bone;//ボーンデータ（名前で参照）

	std::string m_CurAnimationName;
	float m_BlendTime = 1.0f;

	ANIMATIONMATRIX m_AnimationMatrix;
public:
	void LoadModel(const char* FileName);
	void LoadTexture(std::string file_name);
	void LoadAnimation(const char* FileName, const char* AnimationName);

	void Unload();
	void Update(const char* AnimationName,int Frame);
	void Draw();
	void DrawInstance();

	CAnimationModel(){}
	~CAnimationModel(){}
};