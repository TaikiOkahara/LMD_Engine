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