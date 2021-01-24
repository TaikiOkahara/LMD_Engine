#include "director.h"
#include "renderer.h"
#include "skeletalMesh.h"
#include <fstream>


bool LockLowerBody(aiString bone_name);

void CAnimationModel::LoadModel(const char* FileName, D3DXVECTOR3 pos)
{
	const std::string modelPath(FileName);

	
	m_pAiScene = aiImportFile(FileName,
		aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

	assert(m_pAiScene);


	m_pVertexBuffer = new ID3D11Buffer*[m_pAiScene->mNumMeshes];
	m_pIndexBuffer = new ID3D11Buffer*[m_pAiScene->mNumMeshes];
	m_AnimationMatrix = new ANIMATIONMATRIX[m_pAiScene->mNumMeshes];

	//変形後頂点配列生成
	m_vectorDeformVertex = new std::vector<DEFORM_ANIMVERTEX>[m_pAiScene->mNumMeshes];

	//再帰的にボーン生成
	CreateBone(m_pAiScene->mRootNode);

	



	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_pAiScene->mMeshes[m];
		
		ANIMVERTEX_3D* vertex = new ANIMVERTEX_3D[mesh->mNumVertices];
			 
		//頂点バッファ生成
		{

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);// +pos;
				vertex[v].TexturePos = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);//[0]はテクスチャ番号
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].Tangent = D3DXVECTOR3(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);
				vertex[v].Binormal = D3DXVECTOR3(mesh->mBitangents[v].x, mesh->mBitangents[v].y, mesh->mBitangents[v].z);
			}

		}

		//インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned int f = 0; f < mesh->mNumFaces; f++)
			{
				const aiFace* face = &mesh->mFaces[f];

				assert(face->mNumIndices == 3);

				index[f * 3 + 0] = face->mIndices[0];
				index[f * 3 + 1] = face->mIndices[1];
				index[f * 3 + 2] = face->mIndices[2];
			}

			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(unsigned int) * mesh->mNumFaces * 3;
			bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = index;

			RENDERER::GetDevice()->CreateBuffer(&bd, &sd, &m_pIndexBuffer[m]);

			delete[] index;
		}


		//変形後頂点データ初期化
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			DEFORM_ANIMVERTEX deformVertex;
			deformVertex.Posistion = mesh->mVertices[v];
			deformVertex.Normal = mesh->mNormals[v];
			deformVertex.Tangent = mesh->mTangents[v];
			deformVertex.BiNormal = mesh->mBitangents[v];

			deformVertex.BoneNum = 0;

			for (unsigned int b = 0; b < 4; b++)
			{
				deformVertex.BoneName[b] = "";
				deformVertex.BoneWeight[b] = 0.0f;
			}

			m_vectorDeformVertex[m].push_back(deformVertex);
		}

		//ボーンデータ初期化
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_mapBone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//変形後頂点にボーンデータ格納
			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];

				int num = m_vectorDeformVertex[m][weight.mVertexId].BoneNum;

				m_vectorDeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
				m_vectorDeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
				m_vectorDeformVertex[m][weight.mVertexId].BoneIndex[num] = b;
				m_vectorDeformVertex[m][weight.mVertexId].BoneNum++;

				if (num >= 4)
				{
					int a = 0;
				}
			}
		}

		//ボーンインデックス、ウェイトを格納
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			vertex[v].BoneIndex[0] = m_vectorDeformVertex[m][v].BoneIndex[0];
			vertex[v].BoneIndex[1] = m_vectorDeformVertex[m][v].BoneIndex[1];
			vertex[v].BoneIndex[2] = m_vectorDeformVertex[m][v].BoneIndex[2];
			vertex[v].BoneIndex[3] = m_vectorDeformVertex[m][v].BoneIndex[3];

			vertex[v].BoneWeight.x = m_vectorDeformVertex[m][v].BoneWeight[0];
			vertex[v].BoneWeight.y = m_vectorDeformVertex[m][v].BoneWeight[1];
			vertex[v].BoneWeight.z = m_vectorDeformVertex[m][v].BoneWeight[2];
			vertex[v].BoneWeight.w = m_vectorDeformVertex[m][v].BoneWeight[3];

		}


		D3D11_BUFFER_DESC bd;
		ZeroMemory(&bd, sizeof(bd));
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.ByteWidth = sizeof(ANIMVERTEX_3D) * mesh->mNumVertices;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.CPUAccessFlags = 0;

		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.pSysMem = vertex;

		RENDERER::GetDevice()->CreateBuffer(&bd, &sd, &m_pVertexBuffer[m]);

		delete[] vertex;

	}

}

void CAnimationModel::Unload()
{
	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
		m_pVertexBuffer[m]->Release();
		m_pIndexBuffer[m]->Release();
	}

	delete[] m_pVertexBuffer;
	delete[] m_pIndexBuffer;
	delete[] m_AnimationMatrix;

	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_mapTexture)
	{
		if(pair.second != nullptr)
			pair.second->Release();
	}
	m_mapTexture.clear();


	/*for (auto pair : m_Texture) でもいい*/

	for (std::pair<std::string,Animation*> pair : m_mapAnimation)
	{
		if(pair.second->GetScene() != nullptr)
			aiReleaseImport(pair.second->GetScene());

		delete pair.second;
	}

	m_mapAnimation.clear();
	
	
	
	m_mapBone.clear();
	

	
	for (int i = 0;i < m_pAiScene->mNumMeshes; i++)
	{
		std::vector<DEFORM_ANIMVERTEX>().swap(m_vectorDeformVertex[i]);
		m_vectorDeformVertex[i].clear();

	}
	delete[] m_vectorDeformVertex;

	aiReleaseImport(m_pAiScene);

}


void CAnimationModel::LoadAnimation(const char* FileName, const char* AnimationName, int maxFrame,bool rigidity,float speed)
{
	m_mapAnimation[AnimationName] = new Animation(aiImportFile(FileName, aiProcess_ConvertToLeftHanded), maxFrame, rigidity,speed);
	assert(m_mapAnimation[AnimationName]->GetScene());

	m_sCurAnimationName = m_sNexAnimationName = AnimationName;
}

void CAnimationModel::Update()
{
	const int curFrame = (int)m_mapAnimation[m_sCurAnimationName]->GetCurFrame();
	const int nexFrame = (int)m_mapAnimation[m_sNexAnimationName]->GetCurFrame();
	const int curMaxFrame = m_mapAnimation[m_sCurAnimationName]->GetMaxFrame();
	
	
	const aiScene* curScene = m_mapAnimation[m_sCurAnimationName]->GetScene();
	const aiScene* nexScene = m_mapAnimation[m_sNexAnimationName]->GetScene();

	/*if (!curScene->HasAnimations()) { return; }
	if (!nexScene->HasAnimations()) { return; }*/

	//継続更新
	if (m_sCurAnimationName == m_sNexAnimationName)
	{
		//アニメーションデータからボーンマトリクス算出
		aiAnimation* animation = curScene->mAnimations[0];

		for (unsigned int c = 0; c < animation->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation->mChannels[c];
			BONE* bone = &m_mapBone[nodeAnim->mNodeName.C_Str()];

			
			int frot, fpos;

			frot = curFrame % nodeAnim->mNumRotationKeys;//簡易実装
			fpos = curFrame % nodeAnim->mNumPositionKeys;//簡易実装


			aiQuaternion rot = nodeAnim->mRotationKeys[frot].mValue;
			aiVector3D pos = nodeAnim->mPositionKeys[fpos].mValue;


			bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
			
		}

		m_mapAnimation[m_sCurAnimationName]->UpdateFrame();

		//硬直状態なら、時間制限のチェックを行う
		if (m_mapAnimation[m_sCurAnimationName]->GetRigidity())
		{
			if (m_mapAnimation[m_sCurAnimationName]->MotionLock())
				m_CurRigidity = true;
			else
				m_CurRigidity = false;
		}
	}
	//通常→通常　もしくは　通常→硬直　ブレンディング
	else if (!m_mapAnimation[m_sCurAnimationName]->GetRigidity())
	{
		
		//硬直状態なら、時間制限のチェックを行う
		if (m_mapAnimation[m_sNexAnimationName]->GetRigidity())
		{
			m_CurRigidity = true;
		}
				
		//アニメーションデータからボーンマトリクス算出
		aiAnimation* curAnimation = curScene->mAnimations[0];
		aiAnimation* nexAnimation = nexScene->mAnimations[0];
		
		for (unsigned int c = 0; c < curAnimation->mNumChannels; c++)
		{
			aiNodeAnim* curNodeAnim = curAnimation->mChannels[c];
			aiNodeAnim* nexNodeAnim = nexAnimation->mChannels[c];
			BONE* bone = &m_mapBone[curNodeAnim->mNodeName.C_Str()];

			int fc, fn;
			fc = curFrame % curNodeAnim->mNumRotationKeys;//簡易実装
			fn = nexFrame % nexNodeAnim->mNumRotationKeys;//簡易実装

			aiQuaternion rot;
			aiQuaternion::Interpolate(rot, nexNodeAnim->mRotationKeys[fn].mValue, curNodeAnim->mRotationKeys[fc].mValue, 1.0f - m_fBlendTime);

			fc = curFrame % curNodeAnim->mNumPositionKeys;//簡易実装
			fn = nexFrame % nexNodeAnim->mNumPositionKeys;//簡易実装

			aiVector3D pos;

			pos = curNodeAnim->mPositionKeys[fc].mValue * (1.0f - m_fBlendTime) + nexNodeAnim->mPositionKeys[fn].mValue * m_fBlendTime;

			bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);

			

			
		}



		m_mapAnimation[m_sCurAnimationName]->UpdateFrame();
		m_mapAnimation[m_sNexAnimationName]->UpdateFrame();

		//攻撃のアクションはすぐ遷移させる
		if (m_mapAnimation[m_sNexAnimationName]->GetRigidity())
			m_fBlendTime += 0.3f;
		else
			m_fBlendTime += 0.1f;
	
		
		if (m_fBlendTime >= 1)
		{
			m_fBlendTime = 0.0f;
			m_mapAnimation[m_sCurAnimationName]->FrameReset();
			m_sCurAnimationName = m_sNexAnimationName;

		}

		


	}	
	//硬直→通常　もしくは　硬直→硬直　ブレンド  //(!m_mapAnimation[m_sCurAnimationName]->GetRigidity() && m_mapAnimation[m_sNexAnimationName]->GetRigidity())
	else
	{

		if (m_mapAnimation[m_sNexAnimationName]->GetRigidity())
			m_CurRigidity = true;
		else
			m_CurRigidity = false;
		

		//アニメーションデータからボーンマトリクス算出
		aiAnimation* curAnimation = curScene->mAnimations[0];
		aiAnimation* nexAnimation = nexScene->mAnimations[0];

		for (unsigned int c = 0; c < curAnimation->mNumChannels; c++)
		{
			aiNodeAnim* curNodeAnim = curAnimation->mChannels[c];
			aiNodeAnim* nexCodeAnim = nexAnimation->mChannels[c];
			BONE* bone = &m_mapBone[curNodeAnim->mNodeName.C_Str()];

			int fc, fn;
			fc = curMaxFrame % curNodeAnim->mNumRotationKeys;//簡易実装
			fn = nexFrame % nexCodeAnim->mNumRotationKeys;//簡易実装

			aiQuaternion rot;
			aiQuaternion::Interpolate(rot, nexCodeAnim->mRotationKeys[fn].mValue, curNodeAnim->mRotationKeys[fc].mValue, 1.0f - m_fBlendTime);

			fc = curMaxFrame % curNodeAnim->mNumPositionKeys;//簡易実装
			fn = nexFrame % nexCodeAnim->mNumPositionKeys;//簡易実装

			aiVector3D pos;

			pos = curNodeAnim->mPositionKeys[fc].mValue * (1.0f - m_fBlendTime) + nexCodeAnim->mPositionKeys[fn].mValue * m_fBlendTime;

			bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
		}





		m_mapAnimation[m_sNexAnimationName]->UpdateFrame();






		m_fBlendTime += 0.1f;
		if (m_fBlendTime >= 1)
		{
			m_fBlendTime = 0.0f;

			m_mapAnimation[m_sCurAnimationName]->FrameReset();
			m_sCurAnimationName = m_sNexAnimationName;

		}


	}





	//再帰的にボーンマトリクスを更新
	UpdateBoneMatrix(m_pAiScene->mRootNode, aiMatrix4x4());

	//頂点変換
	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_pAiScene->mMeshes[m];

		aiString name = mesh->mName;
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_AnimationMatrix[m].Animation[b]._11 = m_mapBone[bone->mName.C_Str()].Matrix.a1;
			m_AnimationMatrix[m].Animation[b]._12 = m_mapBone[bone->mName.C_Str()].Matrix.a2;
			m_AnimationMatrix[m].Animation[b]._13 = m_mapBone[bone->mName.C_Str()].Matrix.a3;
			m_AnimationMatrix[m].Animation[b]._14 = m_mapBone[bone->mName.C_Str()].Matrix.a4;

			m_AnimationMatrix[m].Animation[b]._21 = m_mapBone[bone->mName.C_Str()].Matrix.b1;
			m_AnimationMatrix[m].Animation[b]._22 = m_mapBone[bone->mName.C_Str()].Matrix.b2;
			m_AnimationMatrix[m].Animation[b]._23 = m_mapBone[bone->mName.C_Str()].Matrix.b3;
			m_AnimationMatrix[m].Animation[b]._24 = m_mapBone[bone->mName.C_Str()].Matrix.b4;

			m_AnimationMatrix[m].Animation[b]._31 = m_mapBone[bone->mName.C_Str()].Matrix.c1;
			m_AnimationMatrix[m].Animation[b]._32 = m_mapBone[bone->mName.C_Str()].Matrix.c2;
			m_AnimationMatrix[m].Animation[b]._33 = m_mapBone[bone->mName.C_Str()].Matrix.c3;
			m_AnimationMatrix[m].Animation[b]._34 = m_mapBone[bone->mName.C_Str()].Matrix.c4;

			m_AnimationMatrix[m].Animation[b]._41 = m_mapBone[bone->mName.C_Str()].Matrix.d1;
			m_AnimationMatrix[m].Animation[b]._42 = m_mapBone[bone->mName.C_Str()].Matrix.d2;
			m_AnimationMatrix[m].Animation[b]._43 = m_mapBone[bone->mName.C_Str()].Matrix.d3;
			m_AnimationMatrix[m].Animation[b]._44 = m_mapBone[bone->mName.C_Str()].Matrix.d4;

		}
	}
}

void CAnimationModel::SetAnimation(const char* AnimationName)
{
	//終わるまで入力できない攻撃
	if (m_CurRigidity)
		return;


	//同じアニメーションなら、継続更新するので、セットは必要ない。
	if (m_sNexAnimationName == AnimationName)
	{
		m_sNexAnimationName = AnimationName;
		return;
	}


	m_fBlendTime = 0.0f;
	m_sNexAnimationName = AnimationName;
	m_mapAnimation[m_sNexAnimationName]->FrameReset();
}


void CAnimationModel::Draw()
{
	//　プリミティブトポロジ設定
	RENDERER::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	

	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{

		RENDERER::GetConstantList().GetStruct<AnimationBuffer>()->Set(m_AnimationMatrix[m]);

		aiMesh* mesh = m_pAiScene->mMeshes[m];

		aiMaterial* material = m_pAiScene->mMaterials[mesh->mMaterialIndex];

		//テクスチャ設定
		aiString path;
		
		//Diffuse
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::GetDeviceContext()->PSSetShaderResources(0, 1, &m_mapTexture[path.data]);
		}
		path.Clear();
		//Normal
		material->GetTexture(aiTextureType_NORMALS, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::GetDeviceContext()->PSSetShaderResources(1, 1, &m_mapTexture[path.data]);
		}
		path.Clear();
		//MRA
		material->GetTexture(aiTextureType_EMISSIVE, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::GetDeviceContext()->PSSetShaderResources(2, 1, &m_mapTexture[path.data]);
		}
		path.Clear();



		//　頂点バッファ設定
		UINT stride = sizeof(ANIMVERTEX_3D);
		UINT offset = 0;
		RENDERER::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer[m], &stride, &offset);

		//　インデックスバッファ設定
		RENDERER::GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);


		//　ポリゴン描画
		RENDERER::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void CAnimationModel::DrawInstance()
{
	//　プリミティブトポロジ設定
	RENDERER::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	

	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_pAiScene->mMeshes[m];

		aiMaterial* material = m_pAiScene->mMaterials[mesh->mMaterialIndex];

		//テクスチャ設定
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		RENDERER::GetDeviceContext()->PSSetShaderResources(0, 1, &m_mapTexture[path.data]);


		//　頂点バッファ設定
		UINT stride = sizeof(ANIMVERTEX_3D);
		UINT offset = 0;
		RENDERER::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_pVertexBuffer[m], &stride, &offset);

		//　インデックスバッファ設定
		RENDERER::GetDeviceContext()->IASetIndexBuffer(m_pIndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);


		//　ポリゴン描画
		RENDERER::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}


void CAnimationModel::CreateBone(aiNode* node)
{
	BONE bone;

	m_mapBone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}

}

void CAnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &m_mapBone[node->mName.C_Str()];

	//マトリクスの乗算順番に注意
	aiMatrix4x4 worldMatrix;

	worldMatrix = matrix;//親のマトリクス
	worldMatrix *= bone->AnimationMatrix;

	bone->Matrix = worldMatrix;
	bone->Matrix *= bone->OffsetMatrix;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		UpdateBoneMatrix(node->mChildren[n], worldMatrix);
	}
}
//　テクスチャ読み込み

void CAnimationModel::LoadTexture(std::string file_name)
{
	//テクスチャ読み込み
	int* texTypeList = new int[3];

	texTypeList[0] = aiTextureType_DIFFUSE;
	texTypeList[1] = aiTextureType_NORMALS;
	texTypeList[2] = aiTextureType_EMISSIVE;//Blender側でMRAをemissiveに入れるとなぜかEMISSIVEで取得可能（なぜかエラーになるから）



	for (int i = 0; i < 3; i++)
	{

		for (unsigned int m = 0; m < m_pAiScene->mNumMaterials; m++)
		{
			aiString path;

			if (m_pAiScene->mMaterials[m]->GetTexture((aiTextureType)texTypeList[i], 0, &path)
				== AI_SUCCESS)
			{
				if (path.data[0] == '*')
				{
					//FBXファイル内から読み込み
					if (m_mapTexture[path.data] == NULL)
					{
						ID3D11ShaderResourceView* texture;
						int id = atoi(&path.data[1]);

						D3DX11CreateShaderResourceViewFromMemory(
							RENDERER::GetDevice(),
							(const unsigned char*)m_pAiScene->mTextures[id]->pcData,
							m_pAiScene->mTextures[id]->mWidth,
							NULL,
							NULL,
							&texture,
							NULL);

						m_mapTexture[path.data] = texture;
					}
				}
				else
				{
					ID3D11ShaderResourceView* texture;

					std::string tex_name;

					tex_name = file_name;
					tex_name += "//";

					tex_name += path.C_Str();

					D3DX11CreateShaderResourceViewFromFile(
						RENDERER::GetDevice(),
						tex_name.c_str(),
						NULL,
						NULL,
						&texture,
						NULL);
					assert(texture);

					m_mapTexture[path.data] = texture;
				}
			}
			else
			{



				m_mapTexture[path.data] = NULL;
			}
		}


	}

	delete[] texTypeList;
}

const char* lock_bone_list[] = {
	"mixamorig:Hips",
	"mixamorig:LeftUpLeg",
	"mixamorig:LeftLeg",
	"mixamorig:LeftFoot",
	"mixamorig:LeftToeBase",
	"mixamorig:LeftToeBase_end",
	"mixamorig:RightUpLeg",
	"mixamorig:RightLeg",
	"mixamorig:RightFoot",
	"mixamorig:RightToeBase",
	"mixamorig:RightToeBase_end",
	NULL
};

bool LockLowerBody(aiString bone_name)
{

	std::string name = bone_name.C_Str();
	for (int i = 0; lock_bone_list[i] != NULL; i++)
	{
		std::string list_name = lock_bone_list[i];
		if (name == list_name)
		{
			return true;
		}
	}

	return false;
}