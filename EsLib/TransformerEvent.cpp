#include "StdAfx.h"
#include "TransformerEvent.h"

ITransformerEvent_Scalar::ITransformerEvent_Scalar(float fTime, float fScalar)
{
	m_fEventTime	= fTime;
	m_fScalar		= fScalar;
}

ITransformerEvent_Pos::ITransformerEvent_Pos(float fTime, GwVector vPos)
{
	m_fEventTime	= fTime;
	m_vPos			= vPos;
}