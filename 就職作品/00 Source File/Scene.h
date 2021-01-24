/*---------------------------------------
*�@scene.h
*
* �S�ẴQ�[���I�u�W�F�N�g���Ǘ�����N���X
* GameObject��std::list�ŊǗ�
*@author�FOkahara Taiki
----------------------------------------*/
#pragma once
#include "director.h"
#include "gameobject.h"
#include "Imgui11.h"
#include "effect.h"

class CScene
{
protected:

	enum LAYER	//GameObject�C���X�^���X�������Ɍ���
	{
		HIDE = 0,//��`�惌�C���[
		DRAW = 1,//��ʓI�ȃI�u�W�F�N�g���C���[
		LIGHT= 2,//���C�e�B���O���C���[
		MAX  = 3,//���C���[��
	};

	

	std::list<CGameObject*> m_GameObject[LAYER::MAX];

	std::list<CEffect*> m_pPostProcessList;
public:	
	CScene(){}
	virtual ~CScene()
	{
		for (int i = 0; i < LAYER::MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Uninit();
				delete object;
			}

			m_GameObject[i].clear();
		}

		for (CEffect* effect : m_pPostProcessList)
		{
			effect->Uninit();
			delete effect;
		}

		m_pPostProcessList.clear();
	}


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

		for (CEffect* effect : m_pPostProcessList)
		{
			effect->Uninit();
			delete effect;
		}

		m_pPostProcessList.clear();
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

		
		for (CEffect* effect : m_pPostProcessList)
		{
			effect->Update();
		}
	}

	virtual void Imgui() {	
		for (int i = 0; i < LAYER::MAX; i++)
		{
			for (CGameObject* object : m_GameObject[i])
			{
				object->Imgui();
			}
		}

		for (CEffect* effect : m_pPostProcessList)
		{
			effect->Imgui();
		}
	}

	virtual void PostProcessDraw(){

		for (CEffect* effect : m_pPostProcessList)
		{
			effect->Draw();
		}
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
				if (typeid(*object) == typeid(T))//�@�^�𒲂ׂ�
				{
					return (T*)object;
				}
			}
		}
		return NULL;
	}

	template <typename T>
	T* AddPostProcess()
	{
		T* effect = new T();
		m_pPostProcessList.push_back(effect);
		effect->Init();

		return effect;
	}

	template <typename T>
	T* GetPostProcess()
	{
		for (CEffect* effect : m_pPostProcessList)
		{
			if (typeid(*effect) == typeid(T))//�@�^�𒲂ׂ�
			{
				return (T*)effect;
			}
		}

		return NULL;
	}
	
};
