#include "director.h"
#include "renderer.h"
#include "SkeletalMesh.h"
#include <fstream>

void CAnimationModel::LoadModel(const char* FileName, D3DXVECTOR3 pos)
{
	const std::string modelPath(FileName);

	//TEXTUREINI textureIni;
	//LoadTextureFile(textureIni, FileName);

	/*m_AiScene = aiImportFile(FileName,
		aiProcessPreset_TargetRealtime_MaxQuality);*/
		//↓テクスチャ座標をDirectX用に変換してくれるので、
		//Y座標を1.0f‐する必要がない
	m_AiScene = aiImportFile(FileName,
		aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

	assert(m_AiScene);

	m_VertexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer*[m_AiScene->mNumMeshes];


	//変形後頂点配列生成
	m_DeformVertex = new std::vector<DEFORM_ANIMVERTEX>[m_AiScene->mNumMeshes];

	//再帰的にボーン生成
	CreateBone(m_AiScene->mRootNode);

	ANIMVERTEX_3D* vertex;

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		//頂点バッファ生成
		{
			 vertex = new ANIMVERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z) + pos;
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexturePos = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);//[0]はテクスチャ番号
				vertex[v].Tangent = D3DXVECTOR3(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);
				vertex[v].Binormal = D3DXVECTOR3(mesh->mBitangents[v].x, mesh->mBitangents[v].y, mesh->mBitangents[v].z);

				//vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}

		}

		//インデックスバッファ生成
		{
			unsigned int* index = new unsigned int[mesh->mNumFaces * 3];

			for (unsigned f = 0; f < mesh->mNumFaces; f++)
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

			RENDERER::m_pDevice->CreateBuffer(&bd, &sd, &m_IndexBuffer[m]);

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
				deformVertex.BoneName[b] = "None";
				deformVertex.BoneWeight[b] = 0.0f;
			}

			m_DeformVertex[m].push_back(deformVertex);
		}

		//ボーンデータ初期化
		for (unsigned int b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_Bone[bone->mName.C_Str()].OffsetMatrix = bone->mOffsetMatrix;

			//変形後頂点にボーンデータ格納
			for (unsigned int w = 0; w < bone->mNumWeights; w++)
			{
				aiVertexWeight weight = bone->mWeights[w];

				int num = m_DeformVertex[m][weight.mVertexId].BoneNum;

				m_DeformVertex[m][weight.mVertexId].BoneWeight[num] = weight.mWeight;
				m_DeformVertex[m][weight.mVertexId].BoneName[num] = bone->mName.C_Str();
				m_DeformVertex[m][weight.mVertexId].BoneIndex[num] = b;
				m_DeformVertex[m][weight.mVertexId].BoneNum++;

				if (num >= 4)
				{
					assert(num >= 4);

				}
			}
		}

		//ボーンインデックス、ウェイトを格納
		for (unsigned int v = 0; v < mesh->mNumVertices; v++)
		{
			vertex[v].BoneIndex[0] = m_DeformVertex[m][v].BoneIndex[0];
			vertex[v].BoneIndex[1] = m_DeformVertex[m][v].BoneIndex[1];
			vertex[v].BoneIndex[2] = m_DeformVertex[m][v].BoneIndex[2];
			vertex[v].BoneIndex[3] = m_DeformVertex[m][v].BoneIndex[3];

			vertex[v].BoneWeight.x = m_DeformVertex[m][v].BoneWeight[0];
			vertex[v].BoneWeight.y = m_DeformVertex[m][v].BoneWeight[1];
			vertex[v].BoneWeight.z = m_DeformVertex[m][v].BoneWeight[2];
			vertex[v].BoneWeight.w = m_DeformVertex[m][v].BoneWeight[3];

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

		RENDERER::m_pDevice->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

		delete[] vertex;
	}

	for (int i = 0; i < ANIMATION_MATRIX_MAX; i++)
	{
		//正規化
		D3DXMatrixIdentity(&m_AnimationMatrix.Animation[i]);
	}
}

void CAnimationModel::Unload()
{
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		m_VertexBuffer[m]->Release();
		m_IndexBuffer[m]->Release();
	}

	delete[] m_VertexBuffer;
	delete[] m_IndexBuffer;

	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_Texture)
	{
		if(pair.second != nullptr)
			pair.second->Release();
	}
	/*for (auto pair : m_Texture) でもいい*/

	for (std::pair<const std::string, const aiScene*> pair : m_Animation)
	{
		if(pair.second != nullptr)
			aiReleaseImport(pair.second);
	}

	aiReleaseImport(m_AiScene);
}


void CAnimationModel::LoadAnimation(const char* FileName, const char* AnimationName)
{
	m_Animation[AnimationName] = aiImportFile(FileName, aiProcess_ConvertToLeftHanded);
	assert(m_Animation[AnimationName]);

	m_CurAnimationName = AnimationName;
}

void CAnimationModel::Update(const char* AnimationName,int Frame)
{
	if (m_CurAnimationName != AnimationName)
	{
		m_BlendTime -= 0.1f;
		if (m_BlendTime <= 0)
		{
			m_BlendTime = 1.0f;
			m_CurAnimationName = AnimationName;
		}
	}


	const aiScene* scene0 = m_Animation[m_CurAnimationName];
	const aiScene* scene1 = m_Animation[AnimationName];

	if (!scene0->HasAnimations()){ return;}
	if (!scene1->HasAnimations()){ return;}


	//アニメーションデータからボーンマトリクス算出
	aiAnimation* animation0 = scene0->mAnimations[0];
	aiAnimation* animation1 = scene1->mAnimations[0];

	for (unsigned int c = 0; c < animation0->mNumChannels; c++)
	{
		aiNodeAnim* nodeAnim0 = animation0->mChannels[c];
		aiNodeAnim* nodeAnim1 = animation1->mChannels[c];
		BONE* bone = &m_Bone[nodeAnim0->mNodeName.C_Str()];

		int f0,f1;
		f0 = Frame % nodeAnim0->mNumRotationKeys;//簡易実装
		f1 = Frame % nodeAnim1->mNumRotationKeys;//簡易実装
		
		aiQuaternion rot;
		aiQuaternion::Interpolate(rot,nodeAnim1->mRotationKeys[f1].mValue,nodeAnim0->mRotationKeys[f0].mValue,m_BlendTime);

		f0 = Frame % nodeAnim0->mNumPositionKeys;//簡易実装
		f1 = Frame % nodeAnim1->mNumPositionKeys;//簡易実装

		aiVector3D pos;
		
		pos.x = nodeAnim0->mPositionKeys[f0].mValue.x * m_BlendTime + nodeAnim1->mPositionKeys[f1].mValue.x * (1.0f - m_BlendTime);
		pos.y = nodeAnim0->mPositionKeys[f0].mValue.y * m_BlendTime + nodeAnim1->mPositionKeys[f1].mValue.y * (1.0f - m_BlendTime);
		pos.z = nodeAnim0->mPositionKeys[f0].mValue.z * m_BlendTime + nodeAnim1->mPositionKeys[f1].mValue.z * (1.0f - m_BlendTime);

		bone->AnimationMatrix = aiMatrix4x4(aiVector3D(1.0f, 1.0f, 1.0f), rot, pos);

	}

	//再帰的にボーンマトリクスを更新
	UpdateBoneMatrix(m_AiScene->mRootNode, aiMatrix4x4());

	//頂点変換
	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];


		for (unsigned int  b = 0; b < mesh->mNumBones; b++)
		{
			aiBone* bone = mesh->mBones[b];

			m_AnimationMatrix.Animation[b]._11 = m_Bone[bone->mName.C_Str()].Matrix.a1;
			m_AnimationMatrix.Animation[b]._12 = m_Bone[bone->mName.C_Str()].Matrix.a2;
			m_AnimationMatrix.Animation[b]._13 = m_Bone[bone->mName.C_Str()].Matrix.a3;
			m_AnimationMatrix.Animation[b]._14 = m_Bone[bone->mName.C_Str()].Matrix.a4;

			m_AnimationMatrix.Animation[b]._21 = m_Bone[bone->mName.C_Str()].Matrix.b1;
			m_AnimationMatrix.Animation[b]._22 = m_Bone[bone->mName.C_Str()].Matrix.b2;
			m_AnimationMatrix.Animation[b]._23 = m_Bone[bone->mName.C_Str()].Matrix.b3;
			m_AnimationMatrix.Animation[b]._24 = m_Bone[bone->mName.C_Str()].Matrix.b4;

			m_AnimationMatrix.Animation[b]._31 = m_Bone[bone->mName.C_Str()].Matrix.c1;
			m_AnimationMatrix.Animation[b]._32 = m_Bone[bone->mName.C_Str()].Matrix.c2;
			m_AnimationMatrix.Animation[b]._33 = m_Bone[bone->mName.C_Str()].Matrix.c3;
			m_AnimationMatrix.Animation[b]._34 = m_Bone[bone->mName.C_Str()].Matrix.c4;

			m_AnimationMatrix.Animation[b]._41 = m_Bone[bone->mName.C_Str()].Matrix.d1;
			m_AnimationMatrix.Animation[b]._42 = m_Bone[bone->mName.C_Str()].Matrix.d2;
			m_AnimationMatrix.Animation[b]._43 = m_Bone[bone->mName.C_Str()].Matrix.d3;
			m_AnimationMatrix.Animation[b]._44 = m_Bone[bone->mName.C_Str()].Matrix.d4;
		}


		RENDERER::SetAnimationMatrix(m_AnimationMatrix);
		
	}

}

void CAnimationModel::Draw()
{
	//　プリミティブトポロジ設定
	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//　マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	RENDERER::SetMaterial(material);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		aiMaterial* material = m_AiScene->mMaterials[mesh->mMaterialIndex];

		//テクスチャ設定
		aiString path;
		/*material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (m_Texture["Diffuse"]){
			RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &m_Texture["Diffuse"]);
		}
		if (m_Texture["Normal"]) {
			RENDERER::m_pDeviceContext->PSSetShaderResources(1, 1, &m_Texture["Normal"]);
		}*/
		//Diffuse
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (m_Texture[path.data]) {
			RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &m_Texture[path.data]);
		}
		path.Clear();
		//Normal
		material->GetTexture(aiTextureType_NORMALS, 0, &path);
		if (m_Texture[path.data]) {
			RENDERER::m_pDeviceContext->PSSetShaderResources(1, 1, &m_Texture[path.data]);
		}


		//　頂点バッファ設定
		UINT stride = sizeof(ANIMVERTEX_3D);
		UINT offset = 0;
		RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		//　インデックスバッファ設定
		RENDERER::m_pDeviceContext->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);


		//　ポリゴン描画
		RENDERER::m_pDeviceContext->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void CAnimationModel::DrawInstance()
{
	//　プリミティブトポロジ設定
	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//　マテリアル設定
	MATERIAL material;
	ZeroMemory(&material, sizeof(material));
	material.Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	material.Ambient = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	RENDERER::SetMaterial(material);

	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		aiMaterial* material = m_AiScene->mMaterials[mesh->mMaterialIndex];

		//テクスチャ設定
		aiString path;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &m_Texture[path.data]);


		//　頂点バッファ設定
		UINT stride = sizeof(ANIMVERTEX_3D);
		UINT offset = 0;
		RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		//　インデックスバッファ設定
		RENDERER::m_pDeviceContext->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);


		//　ポリゴン描画
		RENDERER::m_pDeviceContext->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}


void CAnimationModel::CreateBone(aiNode* node)
{
	BONE bone;

	m_Bone[node->mName.C_Str()] = bone;

	for (unsigned int n = 0; n < node->mNumChildren; n++)
	{
		CreateBone(node->mChildren[n]);
	}

}

void CAnimationModel::UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix)
{
	BONE* bone = &m_Bone[node->mName.C_Str()];

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
		for (unsigned int m = 0; m < m_AiScene->mNumMaterials; m++)
		{
			aiString pathD;
			aiString pathN;

			//Diffuse
			if (m_AiScene->mMaterials[m]->GetTexture(aiTextureType_DIFFUSE, 0, &pathD)
				== AI_SUCCESS)
			{
				if (pathD.data[0] == '*')
				{
					//FBXファイル内から読み込み
					if (m_Texture[pathD.data] == NULL)
					{
						ID3D11ShaderResourceView* texture;
						int id = atoi(&pathD.data[1]);

						D3DX11CreateShaderResourceViewFromMemory(
							RENDERER::m_pDevice,
							(const unsigned char*)m_AiScene->mTextures[id]->pcData,
							m_AiScene->mTextures[id]->mWidth,
							NULL,
							NULL,
							&texture,
							NULL);

						m_Texture[pathD.data] = texture;
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

					m_Texture[pathD.data] = texture;

				}
			}
			else
			{
				m_Texture[pathD.data] = NULL;
			}

			//Normal
			if (m_AiScene->mMaterials[m]->GetTexture(aiTextureType_NORMALS, 0, &pathN)
				== AI_SUCCESS)
			{
				if (pathN.data[0] == '*')
				{
					//FBXファイル内から読み込み
					if (m_Texture[pathN.data] == NULL)
					{
						ID3D11ShaderResourceView* texture;
						int id = atoi(&pathN.data[1]);

						D3DX11CreateShaderResourceViewFromMemory(
							RENDERER::m_pDevice,
							(const unsigned char*)m_AiScene->mTextures[id]->pcData,
							m_AiScene->mTextures[id]->mWidth,
							NULL,
							NULL,
							&texture,
							NULL);

						m_Texture[pathN.data] = texture;
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

					m_Texture[pathN.data] = texture;
				}
			}
			else
			{
				m_Texture[pathN.data] = NULL;
			}
		}
	}
}

#ifdef DEBUG


void CAnimationModel::LoadTexture(std::string file_name)
{
	

	//SetVisualDirectory();
	std::ifstream ifs(file_name);
	std::string str;
	int linecount = 0;
	int texnum = 0;
	int texcount;

	ifs.clear();
	ifs.seekg(0, std::ios::beg);

	while (getline(ifs, str))
	{
		switch (linecount)
		{
			//case 0:
			//	//シェーダー名取得(頂点)
			//	break;

			//case 1:
			//	//シェーダー名取得（ピクセル）
			//	break;

		case 0:
			//テクスチャ数
			texnum = atoi(str.c_str());
			texcount = texnum;
			break;
		default:
			//テクスチャの数分テクスチャ名読み込み

			std::string textureName;
			switch (texnum - texcount)
			{
			case 0:
				SetVisualDirectory();
				textureName = str;
				ID3D11ShaderResourceView* texture;

				D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, textureName.c_str(), NULL, NULL, &texture, NULL);
				m_Texture["Diffuse"] = texture;

				texcount--;
				break;
			case 1:

				SetVisualDirectory();
				textureName = str;
				ID3D11ShaderResourceView* texturenor;

				D3DX11CreateShaderResourceViewFromFile(RENDERER::m_pDevice, textureName.c_str(), NULL, NULL, &texturenor, NULL);
				m_Texture["Normal"] = texturenor;



				ID3D11Texture2D* pNormalTexture;
				if (FAILED(D3DX11CreateTextureFromFile(RENDERER::m_pDevice, textureName.c_str(), NULL, NULL, (ID3D11Resource**)&pNormalTexture, NULL)))
				{
					int a = 0;
				}


				//ノーマルマップのテクスチャーのシェーダーリソースビュー作成
				D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
				ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
				D3D11_TEXTURE2D_DESC desc;
				pNormalTexture->GetDesc(&desc);

				srvDesc.Format = desc.Format;
				srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				srvDesc.Texture2D.MipLevels = desc.MipLevels;
				srvDesc.Texture2D.MostDetailedMip = 0;

				RENDERER::m_pDevice->CreateShaderResourceView(pNormalTexture, &srvDesc, &m_Texture["Normal"]);

				texcount--;

				break;
			}


			break;

		}

		linecount++;
	}

}

#endif // DEBUG