#include "StdAfx.h"
#include "TableMgr.h"

CTableMgr::CTableMgr(void)
{
	m_pTable_Menu = new CTable_Menu();
	m_pTable_GameObject_Tag = new CTable_GameObject_Tag();
}

CTableMgr::~CTableMgr(void)
{
	m_pTable_Menu->Release();
	m_pTable_GameObject_Tag->Release();
}
