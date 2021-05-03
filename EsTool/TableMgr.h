#pragma once

class CTableMgr : public TGwGetInstance<CTableMgr>
{
public:
	CTableMgr(void);
	~CTableMgr(void);

	CTable_Menu				*	GetTable_Menu()				{ return m_pTable_Menu; }
	CTable_GameObject_Tag	*	GetTable_GameObject_Tag()	{ return m_pTable_GameObject_Tag; }

private:
	CTable_Menu				*	m_pTable_Menu;
	CTable_GameObject_Tag	*	m_pTable_GameObject_Tag;
};
