#pragma once

#include "director.h"
#include "gameobject.h"
#include "Imgui11.h"
#include "postProcess.h"

class CScene
{
protected:

	enum LAYER
	{
		HIDE = 0,//非描画レイヤー
		DRAW = 1,//一般的なオブジェクトレイヤー
		LIGHT= 2,//ライティングレイヤー
		MAX  = 3,//レイヤー数
	};

	

	std::list<CGameObject*> m_GameObject[LAYER::MAX];

	CPostProcess* m_pPostProcess;
public:	
	CScene(){}
	virtual ~CScene(){}


	virtual void Init() = 0;

	virtual void UnInit() {
		for (int i = 0; i < LAYER::MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}

			m_GameObject[i].clear();
		}

		if (m_pPostProcess != nullptr)
		{
			m_pPostProcess->Uninit();
			delete m_pPostProcess;
		}
	}

	virtual void Draw() {
		for (int i = 0; i < LAYER::MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Draw();
			}
		}
	}

	virtual void Update() {
		for (int i = 0; i < LAYER::MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Update();
			}
		}

		if (m_pPostProcess != nullptr)
			m_pPostProcess->Update();
	}

	virtual void Imgui() {	
		for (int i = 0; i < LAYER::MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Imgui();
			}
		}

		if (m_pPostProcess != nullptr)
			m_pPostProcess->Imgui();
	}

	virtual void PostProcessDraw(){

		if (m_pPostProcess != nullptr)
			m_pPostProcess->Draw();
	}

	template <typename T>
	T* AddGameObject(int Layer)
	{
		T* gameobject = new T();
		m_GameObject[Layer].push_back(gameobject);
		gameobject->Init();

		return gameobject;
	}

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < LAYER::MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				if (typeid(*object) == typeid(T))//　型を調べる
				{
					return (T*)object;
				}
			}
		}
		return NULL;
	}

	
};
