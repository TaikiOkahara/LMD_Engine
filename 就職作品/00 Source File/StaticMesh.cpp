#include "director.h"
#include "renderer.h"
#include "StaticMesh.h"
#include <fstream>

void StaticMesh::LoadModel(const char* FileName)
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

	

	m_VertexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];
	m_IndexBuffer = new ID3D11Buffer * [m_AiScene->mNumMeshes];


	//変形後頂点配列生成
	m_DeformVertex = new std::vector<DEFORM_VERTEX>[m_AiScene->mNumMeshes];


	for (unsigned int m = 0; m < m_AiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_AiScene->mMeshes[m];

		//頂点バッファ生成
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z) /100;//スケール調整
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].TexturePos = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);//[0]はテクスチャ番号
				vertex[v].Tangent = D3DXVECTOR3(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);
				vertex[v].Binormal = D3DXVECTOR3(mesh->mBitangents[v].x, mesh->mBitangents[v].y, mesh->mBitangents[v].z);
				//vertex[v].Diffuse = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
			}
			
			D3D11_BUFFER_DESC bd;
			ZeroMemory(&bd, sizeof(bd));
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof(VERTEX_3D) * mesh->mNumVertices;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;

			D3D11_SUBRESOURCE_DATA sd;
			ZeroMemory(&sd, sizeof(sd));
			sd.pSysMem = vertex;

			RENDERER::m_pDevice->CreateBuffer(&bd, &sd, &m_VertexBuffer[m]);

			delete[] vertex;
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

	}

	
}

void StaticMesh::Unload()
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
		if (pair.second)
		{
			pair.second->Release();
		}
	}
	/*for (auto pair : m_Texture) でもいい*/


	aiReleaseImport(m_AiScene);
}


void StaticMesh::Update()
{
	

}

void StaticMesh::Draw()
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
		
		//Diffuse
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (m_Texture[path.data]){
			RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &m_Texture[path.data]);
		}
		path.Clear();
		//Normal
		material->GetTexture(aiTextureType_NORMALS, 0, &path);
		if (m_Texture[path.data]) {
			RENDERER::m_pDeviceContext->PSSetShaderResources(1, 1, &m_Texture[path.data]);
		}

		//　頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		//　インデックスバッファ設定
		RENDERER::m_pDeviceContext->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);


		//　ポリゴン描画
		RENDERER::m_pDeviceContext->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void StaticMesh::DrawInstanced(UINT instanceCount)
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
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer[m], &stride, &offset);

		//　インデックスバッファ設定
		RENDERER::m_pDeviceContext->IASetIndexBuffer(m_IndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);


		//　ポリゴン描画
		//RENDERER::m_pDeviceContext->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
		RENDERER::m_pDeviceContext->DrawIndexedInstanced(mesh->mNumFaces * 3, instanceCount,0, 0,0);
	}
}


void StaticMesh::LoadTexture(std::string file_name)
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

//　テクスチャ読み込み
/*void CModel::LoadTexture(std::string file_name)
{
	//std::string mat_file = FindFile("02 Visual File", file_name, ".ini");


	//SetVisualDirectory(file_name);
	//TEXTUREINI Texture;

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

}*/