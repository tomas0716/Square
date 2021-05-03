#include "StdAfx.h"
#include "Helper.h"

IHelper::IHelper(void)
{
}

IHelper::~IHelper(void)
{
}

GwMat	IHelper::GetMatrix(GwVector vPosition, GwVector vRotation, GwVector vScale)
{
	GwMat matWorld;

	matWorld._11 = cosf(vRotation.z) * cosf(vRotation.y) * vScale.x;
	matWorld._12 = sinf(vRotation.z) * cosf(vRotation.y) * vScale.x;
	matWorld._13 = - sinf(vRotation.y) * vScale.x;
	matWorld._14 = 0;
	matWorld._21 = (cosf(vRotation.z) * sinf(vRotation.y) * sinf(vRotation.x) - sinf(vRotation.z) * cosf(vRotation.x)) * vScale.y;
	matWorld._22 = (sinf(vRotation.z) * sinf(vRotation.y) * sinf(vRotation.x) + cosf(vRotation.z) * cosf(vRotation.x)) * vScale.y;
	matWorld._23 = cosf(vRotation.y) * sinf(vRotation.x) * vScale.y;
	matWorld._24 = 0;
	matWorld._31 = (cosf(vRotation.z) * sinf(vRotation.y) * cosf(vRotation.x) + sinf(vRotation.z) * sinf(vRotation.x)) * vScale.z;
	matWorld._32 = (sinf(vRotation.z) * sinf(vRotation.y) * cosf(vRotation.x) - cosf(vRotation.z) * sinf(vRotation.x)) * vScale.z;
	matWorld._33 = cosf(vRotation.y) * cosf(vRotation.x) * vScale.z;
	matWorld._34 = 0;
	matWorld._41 = vPosition.x;
	matWorld._42 = vPosition.y;
	matWorld._43 = vPosition.z;
	matWorld._44 = 1;

	return matWorld;
}

GwMat	IHelper::GetMatrix_ToPosition(GwVector vPosition)
{
	GwMat matWorld;
	D3DXMatrixIdentity(&matWorld);

	matWorld._41 = vPosition.x;
	matWorld._42 = vPosition.y;
	matWorld._43 = vPosition.z;
	matWorld._44 = 1;

	return matWorld;
}

GwMat	IHelper::GetMatrix_ToRotation(GwVector vRotation)
{
	GwMat matWorld;

	matWorld._11 = cosf(vRotation.z) * cosf(vRotation.y);
	matWorld._12 = sinf(vRotation.z) * cosf(vRotation.y);
	matWorld._13 = - sinf(vRotation.y);
	matWorld._14 = 0;
	matWorld._21 = (cosf(vRotation.z) * sinf(vRotation.y) * sinf(vRotation.x) - sinf(vRotation.z) * cosf(vRotation.x));
	matWorld._22 = (sinf(vRotation.z) * sinf(vRotation.y) * sinf(vRotation.x) + cosf(vRotation.z) * cosf(vRotation.x));
	matWorld._23 = cosf(vRotation.y) * sinf(vRotation.x);
	matWorld._24 = 0;
	matWorld._31 = (cosf(vRotation.z) * sinf(vRotation.y) * cosf(vRotation.x) + sinf(vRotation.z) * sinf(vRotation.x));
	matWorld._32 = (sinf(vRotation.z) * sinf(vRotation.y) * cosf(vRotation.x) - cosf(vRotation.z) * sinf(vRotation.x));
	matWorld._33 = cosf(vRotation.y) * cosf(vRotation.x);
	matWorld._34 = 0;
	matWorld._41 = 0;
	matWorld._42 = 0;
	matWorld._43 = 0;
	matWorld._44 = 1;

	return matWorld;
}

GwMat	IHelper::GetMatrix_ToScale(GwVector vScale)
{
	GwMat matWorld;

	matWorld._11 = vScale.x;
	matWorld._12 = 0;
	matWorld._13 = 0;
	matWorld._14 = 0;
	matWorld._21 = 0;
	matWorld._22 = vScale.y;
	matWorld._23 = 0;
	matWorld._24 = 0;
	matWorld._31 = 0;
	matWorld._32 = 0;
	matWorld._33 = vScale.z;
	matWorld._34 = 0;
	matWorld._41 = 0;
	matWorld._42 = 0;
	matWorld._43 = 0;
	matWorld._44 = 1;

	return matWorld;
}