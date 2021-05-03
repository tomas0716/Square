#pragma once

class IHelper
{
public:
	IHelper(void);
	~IHelper(void);

	static GwMat	GetMatrix(GwVector vPosition, GwVector vRotation, GwVector vScale);
	static GwMat	GetMatrix_ToPosition(GwVector vPosition);
	static GwMat	GetMatrix_ToRotation(GwVector vRotation);
	static GwMat	GetMatrix_ToScale(GwVector vScale);
};
