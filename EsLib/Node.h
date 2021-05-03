#pragma once

class INode : public IGwRefObject, public ITypeObject
{
	I_DECLARE_DYNCREATE(INode);
public:
	INode();
	INode(const char * pName);
	INode(const char * pName, UINT nID);
	virtual ~INode();

	virtual void		Update(float fDeltaTime);
	virtual void		Render(float fDeltaTime);
	virtual	void		Render(float fDeltaTime, IGwViewFrustumOct * pFrustum);

	virtual void		SetName(const char * pName);
	virtual string		GetName();

	virtual void		SetID(UINT ID);
	virtual UINT		GetID();

	virtual void		SetParent(INode * pParent);
	virtual INode	*	GetParent();

	virtual void		AddChild(INode * pNode);
	virtual void		RemoveChild(INode * pNode);
	
	virtual int			GetNumChild();
	virtual INode	*	GetChild_byIndex(int Index);

	template<class T>
	void				FindChild_byName(const char * pName, vector<T*> & pChildList);

	template<class T>
	void				FindChild_byID(UINT ID, vector<T*> & pChildList);

	template<class T>
	void				FindChild_AllTreeNode_byName(const char * pName, vector<T*> & pChildList);

	template<class T>
	void				FindChild_AllTreeNode_byID(UINT ID, vector<T*> & pChildList);

	virtual bool		IsChildAllNode(INode * pNode);

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

protected:

	string				m_strName;
	UINT				m_nID;
	INode			*	m_pParent;
	vector<INode*>		m_ChildList;
	vector<INode*>		m_RemoveChildList;
	vector<INode*>		m_AddChildList;

public:

	static UINT			ms_nLastID;
};

#include "Node.inl"