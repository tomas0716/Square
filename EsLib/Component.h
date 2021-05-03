#pragma once

class IGameObject;

class IComponent : public IGwRefObject, public ITypeObject
{
	I_DECLARE_DYNCREATE(IComponent);
public:
	IComponent();
	IComponent(IGameObject * pParent);
	virtual ~IComponent();

	virtual	void		Update(float fDeltaTime)	{ }
	virtual void		Render(float fDeltaTime)	{ }
	virtual void		Render(float fDeltaTime, IGwViewFrustumOct * pFrustum)	{ }

	void				SetParent(IGameObject * pParent);
	IGameObject		*	GetParent();

	void				SetEnable(bool IsEnable);
	bool				IsEnable();

	void				CalcAABB()	{ }

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

protected:

	bool				m_IsEnable;
	IGameObject		*	m_pParentGameObject;
};
