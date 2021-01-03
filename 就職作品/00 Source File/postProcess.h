#pragma once
#include "director.h"

#include "effect.h"


class CPostProcess
{
private:
	std::list<CEffect*> m_Effect;
public:
	CPostProcess() {}
	virtual ~CPostProcess() {}


	virtual void Init() = 0;
	virtual void Uninit()
	{
		for (CEffect* effect : m_Effect)
		{
			effect->Uninit();
			delete effect;
		}

		m_Effect.clear();
	}

	virtual void Update() 
	{
		for (CEffect* effect : m_Effect)
		{
			effect->Update();
		}
	}

	virtual void Draw()
	{
		//
		for (CEffect* effect : m_Effect)
		{
			effect->Draw();
			//RENDERER::PostProcessDraw();
		}
	}

	virtual void Imgui()
	{
		for (CEffect* effect : m_Effect)
		{
			effect->Imgui();
		}
	}

	template <typename T>
	T* AddPostProcess(void)
	{
		T* effect = new T();
		m_Effect.push_back(effect);
		effect->Init();

		return effect;
	}

	template <typename T>
	T* GetPostProcess()
	{
		for (CEffect* effect : m_Effect)
		{
			if (typeid(*effect) == typeid(T))//@Œ^‚ğ’²‚×‚é
			{
				return (T*)effect;
			}
		}

		return NULL;
	}
};