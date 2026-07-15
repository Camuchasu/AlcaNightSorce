#pragma once
#include "../Base/ObjectBase.h"

class Wepon : public ObjectBase 
{
private:

public:
	Wepon(const CVector3D& pos);
	~Wepon();
	void Update()override;
	void Draw()override;

};