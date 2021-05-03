#pragma once

enum eUserDataType
{
	eUserData_Base,
	eUserData_Mesh,
	eUserData_Bone,
	eUserData_Dummy,
};

class IGameObject : public IObject
{
	I_DECLARE_DYNCREATE(IGameObject);
public:
	IGameObject();
	IGameObject(const char * pName);
	virtual ~IGameObject();

	virtual	void			Update(float fDeltaTime);
	virtual void			Render(float fDeltaTime);
	virtual	void			Render(float fDeltaTime, IGwViewFrustumOct * pFrustum);

	bool					AddComponent(IComponent * pComponent);
	bool					RemoveComponent(IComponent * pComponent);
	void					RemoveComponentAll();
	int						GetNumComponent();
	IComponent			*	GetComponent_byIndex(int nIndex);
	IComponent			*	GetComponent_byClassName(string strClassName);

	template<class T>
	void					GetComponent_byClassName(string strClassName, vector<T*> & kComponentList);

	template<class T>
	T					*	GetComponent();

	virtual void			SetColor(GwColor color);

	virtual void			Reset();

	virtual void			SetTag(int nTag);
	virtual int				GetTag();

	virtual void			CalcAABB();

	virtual BYTE	*		LoadFile(BYTE * pData);
	virtual void			SaveFile(IFile & kFile);

protected:

	vector<IComponent*>	m_ComponentList;

	int					m_nTag;
	GwColor				m_Color;
};

#include "GameObject.inl"