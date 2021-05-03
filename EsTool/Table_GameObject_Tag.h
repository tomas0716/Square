#pragma once

class CTable_GameObject_Tag : public IGwRefObject
{
public:
	CTable_GameObject_Tag(void);
	~CTable_GameObject_Tag(void);

	INode	*	GetRoot()	{ return m_pRoot; }

	INode	*	FindNode(int nID);

private:
	void		Load();
	INode	*	FindNode(INode * pNode, int nID);
	INode	*	FindParent(INode * pNode, int nID);

	INode	*	m_pRoot;
};
