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


	class Animation
	{
	private:
		const bool rigidity;//硬直モーションかどうか
		//PRIORITY_LEVEL level;
		const aiScene* scene;
		int maxFrame;
		int curFrame;
	public:

		Animation(const aiScene* sce,int maxf, bool rigid)
			: scene(sce),maxFrame(maxf),rigidity(rigid),curFrame(1){}


		bool MotionLock() {

			if (!rigidity)
				return false;


			return (curFrame < maxFrame) ? true : false;
		}


		bool GetRigidity() {
			return rigidity;
		}

	
		const aiScene* GetScene()
		{
			return scene;
		}

		int GetMaxFrame()
		{
			return maxFrame;
		}

		int GetCurFrame()
		{
			return curFrame;
		}

		void UpdateFrame()
		{
			curFrame++;
		}

		void FrameReset()
		{
			curFrame = 1;
		}
	};

private:
	const aiScene* m_pAiScene = nullptr;
	ID3D11Buffer** m_pVertexBuffer = nullptr;
	ID3D11Buffer** m_pIndexBuffer = nullptr;

	std::map<std::string, ID3D11ShaderResourceView*> m_mapTexture;
	std::map<std::string, Animation*> m_mapAnimation;
	//std::map<std::string, CCondition*> m_mapCondition;//アニメーション遷移クラスのマップ


	const aiScene* m_LockAnimation;

	std::vector<DEFORM_ANIMVERTEX>* m_vectorDeformVertex;//変形後頂点データ
	std::map < const std::string, BONE> m_mapBone;//ボーンデータ（名前で参照）
	
	std::string m_sCurAnimationName;
	std::string m_sNexAnimationName;
	bool m_CurRigidity = false;


	ANIMATIONMATRIX* m_AnimationMatrix = nullptr;


	float m_fBlendTime = 0;
	//int m_iFrame = 1;


	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);
public:
	//bool m_isLock = false;


	void LoadModel(const char* FileName,D3DXVECTOR3 pos);
	void LoadTexture(std::string file_name);

	//LEVEL_0:低、LEVEL_1：高、LEVEL_2：アニメーションロック
	void LoadAnimation(const char* FileName, const char* AnimationName,int maxFrame,bool rigidity);
	

	void Unload();
	void Update();
	void SetAnimation(const char* AnimationName, bool animLock);
	void Draw();
	void DrawInstance();
	bool GetMotionLock() { return m_mapAnimation[m_sCurAnimationName]->GetRigidity(); }
};

class CCondition
{
protected:
	unsigned int m_level;
	const unsigned int m_frameMax;
	const unsigned int m_frameSpeed;
	unsigned int m_frame;

	const bool m_upper;//上半身だけのモーションか
public:
	CCondition(unsigned int level, unsigned int frame, unsigned int speed, bool upper)
		: m_level(level), m_frameMax(frame), m_frameSpeed(speed), m_upper(upper), m_frame(0) {}
	virtual ~CCondition() {}

	bool MotionLock()
	{
		return (m_frame >= m_frameMax) ? false : true;
	};

	void FrameUpdate()
	{
		m_frame += m_frameSpeed;

	}

	void FrameReset()
	{
		m_frame = 0;
	}

	bool GetLowerLock()
	{
		return m_upper;
	}

	unsigned int GetFrameMax()
	{
		return m_frameMax;
	}
	unsigned int GetCurFrame()
	{
		return m_frame;
	}

	unsigned int GetLevel()
	{
		return m_level;
	}
};