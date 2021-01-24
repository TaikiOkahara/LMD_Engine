/*---------------------------------------
*�@effect.h
*
* 1�̃|�X�g�v���Z�X���N���X������C���^�[�t�F�[�X
*@author�FOkahara Taiki
----------------------------------------*/
#pragma once

class CEffect
{
protected:

public:
	CEffect() {}
	virtual ~CEffect() {}

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Imgui() {}
};