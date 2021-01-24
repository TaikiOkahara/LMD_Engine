#pragma once


#pragma warning(disable : 4819)
#include "../05 Resource File/assimp/cimport.h"
#include "../05 Resource File/assimp/scene.h"
#include "../05 Resource File/assimp/postprocess.h"
#include "../05 Resource File/assimp/matrix4x4.h"
#pragma comment (lib,"assimp.lib")

#include "renderer.h"

class CAnimationModel
{
	//ボーン構造体
	struct BONE
	{
		aiMatrix4x4 Matrix;
		aiMatrix4x4 AnimationMatrix;
		aiMatrix4x4 OffsetMatrix;
	};

	//アニメーション頂点情報
	struct DEFORM_ANIMVERTEX
	{
		aiVector3D Posistion;
		aiVector3D Normal;
		aiVector3D Tangent;
		aiVector3D BiNormal;

		unsigned int BoneIndex[4];
		unsigned int BoneNum;
		std::string  BoneName[4];
		float		 BoneWeight[4];

	};


	class Animation
	{
	private:
		const bool rigidity;//硬直モーションかどうか
		const aiScene* scene;
		const int maxFrame;
		float curFrame;
		const float FrameSpeed;
	public:

		Animation(const aiScene* sce,int maxf, bool rigid,float sp)
			: scene(sce),maxFrame(maxf),rigidity(rigid),curFrame(1.0f),FrameSpeed(sp){}

		bool MotionLock() {

			if (!rigidity)
				return false;

			return (curFrame < maxFrame) ? true : false;
		}


		bool	GetRigidity() {return rigidity;}
		const aiScene* GetScene(){return scene;}
		int		GetMaxFrame(){return maxFrame;}
		float	GetCurFrame(){return curFrame;}

		void FrameReset(){curFrame = 1.0f;}
		void UpdateFrame()
		{
			if (rigidity && curFrame >= maxFrame)
				return;

			curFrame += FrameSpeed;
		}
	};

private:
	const aiScene* m_pAiScene = nullptr;
	ID3D11Buffer** m_pVertexBuffer = nullptr;
	ID3D11Buffer** m_pIndexBuffer = nullptr;

	std::map<std::string, ID3D11ShaderResourceView*> m_mapTexture;
	std::map<std::string, Animation*> m_mapAnimation;


	std::vector<DEFORM_ANIMVERTEX>* m_vectorDeformVertex;//変形後頂点データ
	std::map < const std::string, BONE> m_mapBone;//ボーンデータ（名前で参照）
	
	std::string m_sCurAnimationName;
	std::string m_sNexAnimationName;
	bool m_CurRigidity = false;//現アニメーションロック状態


	ANIMATIONMATRIX* m_AnimationMatrix = nullptr;


	float m_fBlendTime = 0;//ブレンディング率(0～1)

	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);
public:
	void LoadModel(const char* FileName,D3DXVECTOR3 pos);
	void LoadTexture(std::string file_name);
	void LoadAnimation(const char* FileName, const char* AnimationName,int maxFrame,bool rigidity,float speed);
	

	void Unload();
	void Update();
	void SetAnimation(const char* AnimationName);
	void Draw();
	void DrawInstance();
	bool GetMotionLock() { return m_mapAnimation[m_sCurAnimationName]->GetRigidity(); }
};
