#pragma once
#include "Base/ObjectBase.h"

class RubbleFallArea : public ObjectBase
{
public:

	RubbleFallArea(const CVector3D& pos, float rad);

	void Update() override;
	void Render() override;

private:
	CModel* mp_model;
};