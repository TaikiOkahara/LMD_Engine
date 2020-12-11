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


	//変形後頂点配列生成
	m_vectorDeformVertex = new std::vector<DEFORM_VERTEX>[m_pAiScene->mNumMeshes];


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

			RENDERER::m_pDevice->CreateBuffer(&bd, &sd, &m_ppVertexBuffer[m]);

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

			RENDERER::m_pDevice->CreateBuffer(&bd, &sd, &m_ppIndexBuffer[m]);

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
	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	

	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
		aiMesh* mesh = m_pAiScene->mMeshes[m];

		aiMaterial* material = m_pAiScene->mMaterials[mesh->mMaterialIndex];

		//テクスチャ設定
		aiString path;
		
		//Diffuse
		material->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		if (m_mapTexture[path.data]){
			RENDERER::m_pDeviceContext->PSSetShaderResources(0, 1, &m_mapTexture[path.data]);
		}
		path.Clear();
		//Normal
		material->GetTexture(aiTextureType_NORMALS, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::m_pDeviceContext->PSSetShaderResources(1, 1, &m_mapTexture[path.data]);
		}
		path.Clear();
		//Roughness
		material->GetTexture(aiTextureType_SHININESS, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::m_pDeviceContext->PSSetShaderResources(2, 1, &m_mapTexture[path.data]);
		}
		path.Clear();
		//metallic
		material->GetTexture(aiTextureType_EMISSIVE, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::m_pDeviceContext->PSSetShaderResources(3, 1, &m_mapTexture[path.data]);
		}
		path.Clear();


		//　頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_ppVertexBuffer[m], &stride, &offset);

		//　インデックスバッファ設定
		RENDERER::m_pDeviceContext->IASetIndexBuffer(m_ppIndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);


		//　ポリゴン描画
		RENDERER::m_pDeviceContext->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
	}
}

void StaticMesh::DrawInstanced(UINT instanceCount)
{
	//　プリミティブトポロジ設定
	RENDERER::m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	for (unsigned int m = 0; m < m_pAiScene->mNumMeshes; m++)
	{
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
		path.Clear();
		//Roughness
		material->GetTexture(aiTextureType_SHININESS, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::m_pDeviceContext->PSSetShaderResources(2, 1, &m_mapTexture[path.data]);
		}
		path.Clear();
		//metallic
		material->GetTexture(aiTextureType_EMISSIVE, 0, &path);
		if (m_mapTexture[path.data]) {
			RENDERER::m_pDeviceContext->PSSetShaderResources(3, 1, &m_mapTexture[path.data]);
		}
		path.Clear();


		//　頂点バッファ設定
		UINT stride = sizeof(VERTEX_3D);
		UINT offset = 0;
		RENDERER::m_pDeviceContext->IASetVertexBuffers(0, 1, &m_ppVertexBuffer[m], &stride, &offset);

		//　インデックスバッファ設定
		RENDERER::m_pDeviceContext->IASetIndexBuffer(m_ppIndexBuffer[m], DXGI_FORMAT_R32_UINT, 0);


		//　ポリゴン描画
		//RENDERER::m_pDeviceContext->DrawIndexed(mesh->mNumFaces * 3, 0, 0);
		RENDERER::m_pDeviceContext->DrawIndexedInstanced(mesh->mNumFaces * 3, instanceCount,0, 0,0);
	}
}


void StaticMesh::LoadTexture(std::string file_name)
{
	//テクスチャ読み込み
	std::map<int, aiTextureType> texTypeList;

	texTypeList[0] = aiTextureType_DIFFUSE;
	texTypeList[1] = aiTextureType_NORMALS;
	texTypeList[2] = aiTextureType_SHININESS;//roughnessをなぜかSHININESSで取得可能（なぜかエラーになるから）
	texTypeList[3] = aiTextureType_EMISSIVE;//Blender側でmetallicをemissiveに入れるとなぜかEMISSIVEで取得可能（なぜかエラーになるから）



	for (int i = 0; i < texTypeList.size(); i++)
	{

		for (unsigned int m = 0; m < m_pAiScene->mNumMaterials; m++)
		{
			aiString path;

			if (m_pAiScene->mMaterials[m]->GetTexture(texTypeList[i], 0, &path)
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
							RENDERER::m_pDevice,
							(const unsigned char*)m_pAiScene->mTextures[id]->pcData,
							m_pAiScene->mTextures[id]->mWidth,
							NULL,
							NULL,
							&texture,
							NULL);

						m_mapTexture[path.data] = texture;
					}
				}
			}
			else
			{
				m_mapTexture[path.data] = NULL;
			}
		}

	
	}


	/*for (unsigned int m = 0; m < m_pAiScene->mNumMaterials; m++)
	{
		aiString pathD;
		aiString pathN;





		//aiTextureType::aiTextureType_UNKNOWN
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
	*/

	texTypeList.clear();

}
