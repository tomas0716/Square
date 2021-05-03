#include "stdafx.h"
#include "Component_TransInfo.h"

I_IMPLEMENT_DYNCREATE(ITransInfo, ISingletonObject::GetTypeModule())
I_IMPLEMENT_DYNCREATE(ITransScaleInfo, ISingletonObject::GetTypeModule())
I_IMPLEMENT_DYNCREATE(ITransColorInfo, ISingletonObject::GetTypeModule())
I_IMPLEMENT_DYNCREATE(ITransAlphaInfo, ISingletonObject::GetTypeModule())
I_IMPLEMENT_DYNCREATE(ITransRotationInfo, ISingletonObject::GetTypeModule())
I_IMPLEMENT_DYNCREATE(ITransSequenceAnimInfo, ISingletonObject::GetTypeModule())

ITransInfo::ITransInfo() : m_fTime(0) { }

ITransScaleInfo::ITransScaleInfo() : m_vTransScale(GwVector(1,1,1))	{ }

ITransColorInfo::ITransColorInfo() : m_clrColor(GwColor(0xffffffff))	{ }

ITransAlphaInfo::ITransAlphaInfo() : m_fAlpha(1)	{ }

ITransRotationInfo::ITransRotationInfo() : m_vRotationAxis(GwVector(0,1,0)), m_fSpeed(10)	{ }

ITransSequenceAnimInfo::ITransSequenceAnimInfo() : m_strMediaPath(""), m_strFileName("")		{ }