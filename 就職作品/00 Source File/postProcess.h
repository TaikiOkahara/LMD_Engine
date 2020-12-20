#pragma once
#include "director.h"

#include "effect.h"


class CPostProcess
{
private:
	std::list<CEffect*> m_Effect;
public:
	
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void Imgui();

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
			if (typeid(*effect) == typeid(T))//Å@å^Çí≤Ç◊ÇÈ
			{
				return (T*)effect;
			}
		}

		return NULL;
	}
};