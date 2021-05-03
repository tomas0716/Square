#pragma once

//////////////////////////////////////////////////////////////////////////
// IComponent_Trans

class IComponent_Trans : public IComponent
{
	I_DECLARE_DYNCREATE(IComponent_Trans);
public:
	IComponent_Trans();
	IComponent_Trans(IGameObject * pParent);
	virtual ~IComponent_Trans();

	virtual void			AddEvent(ITransInfo * pEvent);
	virtual void			RemoveEvent(ITransInfo * pEvent);
	virtual bool			RemoveEvent_byIndex(int nIndex);
	virtual int				GetNumEvent();
	virtual ITransInfo	*	GetEvent_byIndex(int nIndex);

	virtual void			Sort();

	virtual BYTE		*	LoadFile(BYTE * pData);
	virtual void			SaveFile(IFile & kFile);

protected:

	vector<ITransInfo*>		m_EventInfoList;
};

//////////////////////////////////////////////////////////////////////////
// IComponent_Trans_Scale

class IComponent_Trans_Scale : public IComponent_Trans
{
	I_DECLARE_DYNCREATE(IComponent_Trans_Scale);
public:
	IComponent_Trans_Scale();
	IComponent_Trans_Scale(IGameObject * pParent);

	virtual void			AddEvent(ITransInfo * pEvent);
};

//////////////////////////////////////////////////////////////////////////
// IComponent_Trans_Color

class IComponent_Trans_Color : public IComponent_Trans
{
	I_DECLARE_DYNCREATE(IComponent_Trans_Color);
public:
	IComponent_Trans_Color();
	IComponent_Trans_Color(IGameObject * pParent);

	virtual void			AddEvent(ITransInfo * pEvent);
};

//////////////////////////////////////////////////////////////////////////
// IComponent_Trans_Alpha

class IComponent_Trans_Alpha : public IComponent_Trans
{
	I_DECLARE_DYNCREATE(IComponent_Trans_Alpha);
public:
	IComponent_Trans_Alpha();
	IComponent_Trans_Alpha(IGameObject * pParentObject);

	virtual void			AddEvent(ITransInfo * pEvent);
};

//////////////////////////////////////////////////////////////////////////
// IComponent_Trans_Rotation

class IComponent_Trans_Rotation : public IComponent_Trans
{
	I_DECLARE_DYNCREATE(IComponent_Trans_Rotation);
public:
	IComponent_Trans_Rotation();
	IComponent_Trans_Rotation(IGameObject * pParent);

	virtual void			AddEvent(ITransInfo * pEvent);
};

//////////////////////////////////////////////////////////////////////////
// IComponent_Trans_AnimSequence

class IComponent_Trans_AnimSequence : public IComponent_Trans
{
	I_DECLARE_DYNCREATE(IComponent_Trans_AnimSequence);
public:
	IComponent_Trans_AnimSequence();
	IComponent_Trans_AnimSequence(IGameObject * pParent);

	virtual void			AddEvent(ITransInfo * pEvent);
};