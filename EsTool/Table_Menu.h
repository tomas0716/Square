#pragma once

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTable_Menu_Item

class CTable_Menu_Item : public INode
{
public:
	CTable_Menu_Item(const char * pszName, const char * pszFunction);
	~CTable_Menu_Item();

	const char	*	GetFunction()								{ return m_strFunction.c_str(); }

	void			SetSyskey(const char * pszSysKey)			{ m_strSyskey = pszSysKey; }
	const char	*	GetSyskey()									{ return m_strSyskey.c_str(); }

	void			SetChar(char sChar)							{ m_Char = sChar; }
	char			GetChar()									{ return m_Char; }

	void			SetShortKeyDesc(const char * pszDesc)		{ m_strShortKeyDesc = pszDesc; }
	const char	*	GetShortKeyDesc()							{ return m_strShortKeyDesc.c_str(); }

private:

	string			m_strFunction;
	string			m_strSyskey;
	char			m_Char;
	string			m_strShortKeyDesc;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CTable_Menu

class CTable_Menu : public IGwRefObject
{
public:
	CTable_Menu(void);
	~CTable_Menu(void);

	INode	*	GetRoot()	{ return m_pRoot; }

private:
	void		Load();
	INode	*	FindParent(INode * pNode, int nIndex);

	INode	*	m_pRoot;
};
