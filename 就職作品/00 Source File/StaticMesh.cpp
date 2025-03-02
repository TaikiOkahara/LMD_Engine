#include "director.h"
#include "renderer.h"
#include "staticMesh.h"
#include <fstream>

void StaticMesh::LoadModel(const char* FileName)
{
	const std::string modelPath(FileName);

	m_pAiScene = aiImportFile(FileName,
		aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_ConvertToLeftHanded);

	assert(m_pAiScene);

	

	m_ppVertexBuffer = new ID3D11Buffer * [m_pAiScene->mNumMeshes];
	m_ppIndexBuffer = new ID3D11Buffer * [m_pAiScene->mNumMeshes];


	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_pAiScene->mMeshes[m];

		//頂点バッファ生成
		{
			VERTEX_3D* vertex = new VERTEX_3D[mesh->mNumVertices];

			for (unsigned int v = 0; v < mesh->mNumVertices; v++)
			{
				vertex[v].Position = D3DXVECTOR3(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z);
				vertex[v].TexturePos = D3DXVECTOR2(mesh->mTextureCoords[0][v].x, mesh->mTextureCoords[0][v].y);//[0]はテクスチャ番号
				vertex[v].Normal = D3DXVECTOR3(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z);
				vertex[v].Tangent = D3DXVECTOR3(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z);
				vertex[v].Binormal = D3DXVECTOR3(mesh->mBitangents[v].x, mesh->mBitangents[v].y, mesh->mBitangents[v].z);
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

			RENDERER::GetDevice()->CreateBuffer(&bd, &sd, &m_ppVertexBuffer[m]);

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

			RENDERER::GetDevice()->CreateBuffer(&bd, &sd, &m_ppIndexBuffer[m]);

			delete[] index;
		}

	}

	
}

void StaticMesh::Unload()
{
	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
		m_ppVertexBuffer[m]->Release();
		m_ppIndexBuffer[m]->Release();
	}

	delete[] m_ppVertexBuffer;
	delete[] m_ppIndexBuffer;

	for (std::pair<const std::string, ID3D11ShaderResourceView*> pair : m_mapTexture)
	{
		if (pair.second)
		{
			pair.second->Release();
		}
	}
	/*for (auto pair : m_Texture) でもいい*/

	aiReleaseImport(m_pAiScene);
}


void StaticMesh::Update()
{
	

}

void StaticMesh::Draw()
{
	//　プリミティブトポロジ設定
	RENDERER::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	

	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_pAiScene->mMeshes[m];

		aiMaterial* material = m_pAiScene->mMaterials[mesh->mMaterialIndex];

		//テクスチャ設定
		aiString path;
		
		//Diffuse
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (m_mapTexture[path.data]){
			RENDERER::GetDeviceContext()->PSSetShaderResources(0, 1, &m_mapTexture[path.data]);
		}
		path.Clear();
		//Normal
		material->GetTexture(aiTextureType_NORMALS, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::GetDeviceContext()->PSSetShaderResources(1, 1, &m_mapTexture[path.data]);
		}
		path.Clear();
		//Roughness
		material->GetTexture(aiTextureType_SHININESS, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::GetDeviceContext()->PSSetShaderResources(2, 1, &m_mapTexture[path.data]);
		}
		path.Clear();
		//metallic
		material->GetTexture(aiTextureType_EMISSIVE, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::GetDeviceContext()->PSSetShaderResources(3, 1, &m_mapTexture[path.data]);
		}
		path.Clear();


		//　頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		RENDERER::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_ppVertexBuffer[m], &stride, &offset);

		//　インデックスバッファ設定
		RENDERER::GetDeviceContext()->IASetIndexBuffer(m_ppIndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);


		//　ポリゴン描画
		RENDERER::GetDeviceContext()->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void StaticMesh::DrawInstanced(UINT instanceCount)
{
	//　プリミティブトポロジ設定
	RENDERER::GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
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


		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		RENDERER::GetDeviceContext()->IASetVertexBuffers(0, 1, &m_ppVertexBuffer[m], &stride, &offset);
		RENDERER::GetDeviceContext()->IASetIndexBuffer(m_ppIndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);

		RENDERER::GetDeviceContext()->DrawIndexedInstanced(mesh->mNumFaces * 3, instanceCount,0, 0,0);
	}
}


void StaticMesh::LoadTexture(std::string file_name)
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
