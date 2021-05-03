#include "StdAfx.h"
#include "UIListViewItemInfo.h"

CUIListViewItemInfo::CUIListViewItemInfo(const char * pszName, IGwRefObject * pParameta)
: m_strName(pszName), m_pParameta(pParameta), m_eState(eListViewItem_State_None)
{
	if( m_pParameta == NULL )	m_pParameta = new IGwRefObject();
	else						m_pParameta->AddRef();
}

CUIListViewItemInfo::~CUIListViewItemInfo(void)
{
	m_pParameta->Release();
}

void				CUIListViewItemInfo::SetName(const char * pszName)
{
	m_strName = pszName;
}

const char		*	CUIListViewItemInfo::GetName()
{
	return m_strName.c_str();
}

void				CUIListViewItemInfo::SetParameta(IGwRefObject * pParameta)
{
	m_pParameta->Release();
	m_pParameta = pParameta;

	if( m_pParameta == NULL )	m_pParameta = new IGwRefObject();
	else						m_pParameta->AddRef();
}

IGwRefObject	*	CUIListViewItemInfo::GetParameta()
{
	return m_pParameta;
}

void				CUIListViewItemInfo::SetListViewItem_State(eListViewItem_State eState)
{
	m_eState = eState;
}

eListViewItem_State	CUIListViewItemInfo::GetListViewItem_State()
{
	return m_eState;
}