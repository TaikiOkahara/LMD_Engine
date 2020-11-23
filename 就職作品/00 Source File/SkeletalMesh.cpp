#include "director.h"
#include "renderer.h"
#include "skeletalMesh.h"
#include <fstream>

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

	ANIMVERTEX_3D* vertex;

	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_pAiScene->mMeshes[m];

		vertex = new ANIMVERTEX_3D[mesh->mNumVertices];
			 
		//頂点バッファ生成
		{

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);// +pos;
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexturePos = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);//[0]はテクスチャ番号
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

			RENDERER::m_pDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer[m]);

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

		RENDERER::m_pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer[m]);

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
	/*for (auto pair : m_Texture) でもいい*/

	for (std::pair<const std::string, const aiScene*> pair : m_mapAnimation)
	{
		if(pair.second != nullptr)
			aiReleaseImport(pair.second);
	}

	aiReleaseImport(m_pAiScene);
}


void CAnimationModel::LoadAnimation(const char* FileName, const char* AnimationName)
{
	m_mapAnimation[AnimationName] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_mapAnimation[AnimationName]);

	m_sCurAnimationName = AnimationName;
}

void CAnimationModel::Update(const char* AnimationName,int Frame)
{
	if (m_sCurAnimationName != AnimationName)
	{
		m_fBlendTime -= 0.1f;
		if (m_fBlendTime <= 0)
		{
			m_fBlendTime = 1.0f;
			m_sCurAnimationName = AnimationName;
		}
	}

	


	const aiScene* scene0 = m_mapAnimation[m_sCurAnimationName];
	const aiScene* scene1 = m_mapAnimation[AnimationName];

	if (!scene0->HasAnimations()){ return;}
	if (!scene1->HasAnimations()){ return;}


	

	if (m_sCurAnimationName == AnimationName)
	{
		//アニメーションデータからボーンマトリクス算出
		aiAnimation* animation = scene0->mAnimations[0];

		for (unsigned int c = 0; c < animation->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim = animation->mChannels[c];

			BONE* bone = &m_mapBone[nodeAnim->mNodeName.C_Str()];

			int frot,fpos;
			
			frot = Frame % nodeAnim->mNumRotationKeys;//簡易実装
			fpos = Frame % nodeAnim->mNumPositionKeys;//簡易実装


			aiQuaternion rot = nodeAnim->mRotationKeys[frot].mValue;
			aiVector3D pos = nodeAnim->mPositionKeys[fpos].mValue;


			bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);
		}
	}
	else
	{
		//アニメーションデータからボーンマトリクス算出
		aiAnimation* animation0 = scene0->mAnimations[0];
		aiAnimation* animation1 = scene1->mAnimations[0];

		for (unsigned int c = 0; c < animation0->mNumChannels; c++)
		{
			aiNodeAnim* nodeAnim0 = animation0->mChannels[c];
			aiNodeAnim* nodeAnim1 = animation1->mChannels[c];
			BONE* bone = &m_mapBone[nodeAnim0->mNodeName.C_Str()];

			int f0, f1;
			f0 = Frame % nodeAnim0->mNumRotationKeys;//簡易実装
			f1 = Frame % nodeAnim1->mNumRotationKeys;//簡易実装

			aiQuaternion rot;
			aiQuaternion::Interpolate(rot, nodeAnim1->mRotationKeys[f1].mValue, nodeAnim0->mRotationKeys[f0].mValue, m_fBlendTime);

			f0 = Frame % nodeAnim0->mNumPositionKeys;//簡易実装
			f1 = Frame % nodeAnim1->mNumPositionKeys;//簡易実装

			aiVector3D pos;

			pos = nodeAnim0->mPositionKeys[f0].mValue * m_fBlendTime + nodeAnim1->mPositionKeys[f1].mValue * (1.0f - m_fBlendTime);

			bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);

		}
	}

	

	//再帰的にボーンマトリクスを更新
	UpdateBoneMatrix(m_pAiScene->mRootNode, aiMatrix4x4());

	//頂点変換
	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_pAiScene->mMeshes[m];


		for (unsigned int  b = 0; b < mesh->mNumBones; b++)
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

void CAnimationModel::Draw()
{
	//　プリミティブトポロジ設定
	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	

	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{

		RENDERER::SetAnimationMatrix(m_AnimationMatrix[m]);

		aiMesh* mesh = m_pAiScene->mMeshes[m];

		aiMaterial* material = m_pAiScene->mMaterials[mesh->mMaterialIndex];

		//テクスチャ設定
		aiString path;
		
		//Diffuse
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &m_mapTexture[path.data]);
		}
		path.Clear();
		//Normal
		material->GetTexture(aiTextureType_NORMALS, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::m_pDeviceContext->PSSetShaderResources(1, 1, &m_mapTexture[path.data]);
		}


		//　頂点バッファ設定
		UINT stride = sizeof(ANIMVERTEX_3D);
		UINT offset = 0;
		RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer[m], &stride, &offset);

		//　インデックスバッファ設定
		RENDERER::m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);


		//　ポリゴン描画
		RENDERER::m_pDeviceContext->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void CAnimationModel::DrawInstance()
{
	//　プリミティブトポロジ設定
	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	

	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_pAiScene->mMeshes[m];

		aiMaterial* material = m_pAiScene->mMaterials[mesh->mMaterialIndex];

		//テクスチャ設定
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &m_mapTexture[path.data]);


		//　頂点バッファ設定
		UINT stride = sizeof(ANIMVERTEX_3D);
		UINT offset = 0;
		RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer[m], &stride, &offset);

		//　インデックスバッファ設定
		RENDERER::m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);


		//　ポリゴン描画
		RENDERER::m_pDeviceContext->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
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
	{
		for (unsigned int m = 0; m < m_pAiScene->mNumMaterials; m++)
		{
			aiString pathD;
			aiString pathN;

			//Diffuse
			if (m_pAiScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &pathD)
				== AI_SUCCESS)
			{
				if (pathD.data[0] == '*')
				{
					//FBXファイル内から読み込み
					if (m_mapTexture[pathD.data] == NULL)
					{
						ID3D11ShaderResourceView* texture;
						int id = atoi(&pathD.data[1]);

						D3DX11CreateShaderResourceViewFromMemory(
							RENDERER::m_pDevice,
							(const unsigned char*)m_pAiScene->mTextures[id]->pcData,
							m_pAiScene->mTextures[id]->mWidth,
							NULL,
							NULL,
							&texture,
							NULL);

						m_mapTexture[pathD.data] = texture;
					}
				}
				else
				{
					//外部ファイルから読み込み

					//テクスチャの数分テクスチャ名読み込み

					std::string textureName;

					//SetVisualDirectory();
					//textureName = str;

					ID3D11ShaderResourceView* texture;
					//std::string texturename;
					textureName = file_name;
					textureName += "//";
					textureName += pathD.data;

					D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, textureName.c_str(), NULL, NULL, &texture, NULL);

					m_mapTexture[pathD.data] = texture;

				}
			}
			else
			{
				m_mapTexture[pathD.data] = NULL;
			}

			//Normal
			if (m_pAiScene->mMaterials[m]->GetTexture(aiTextureType_NORMALS, 0, &pathN)
				== AI_SUCCESS)
			{
				if (pathN.data[0] == '*')
				{
					//FBXファイル内から読み込み
					if (m_mapTexture[pathN.data] == NULL)
					{
						ID3D11ShaderResourceView* texture;
						int id = atoi(&pathN.data[1]);

						D3DX11CreateShaderResourceViewFromMemory(
							RENDERER::m_pDevice,
							(const unsigned char*)m_pAiScene->mTextures[id]->pcData,
							m_pAiScene->mTextures[id]->mWidth,
							NULL,
							NULL,
							&texture,
							NULL);

						m_mapTexture[pathN.data] = texture;
					}
				}
				else
				{
					//外部ファイルから読み込み

					//テクスチャの数分テクスチャ名読み込み

					std::string textureName;

					//SetVisualDirectory();
					//textureName = str;

					ID3D11ShaderResourceView* texture;

					textureName = file_name;
					textureName += "//";
					textureName += pathN.data;

					D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, textureName.c_str(), NULL, NULL, &texture, NULL);

					m_mapTexture[pathN.data] = texture;
				}
			}
			else
			{
				m_mapTexture[pathN.data] = NULL;
			}
		}
	}
}
