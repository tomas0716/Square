#pragma once

//////////////////////////////////////////////////////////////////////////
// IParticleObjectInfo

class IParticleObjectInfo : public IGwRefObject, public ITypeObject
{
	I_DECLARE_DYNCREATE(IParticleObjectInfo);
public:
	IParticleObjectInfo(void);
	virtual ~IParticleObjectInfo(void);

	virtual void		CreateObject()								{ }
	virtual void		SetObject(IGameObject * pGameObject)		{ }
	virtual void		SetRenderState(eEsRenderState eRenderState)	{ m_eRenderState = eRenderState; }

	virtual BYTE	*	LoadFile(BYTE * pData)		{ return NULL; }
	virtual void		SaveFile(IFile & kFile)		{ }

public:
	string				m_strGameObjectName;
	eEsRenderState		m_eRenderState;
};

//////////////////////////////////////////////////////////////////////////
// Billboard

class IParticleObjectInfo_Billboard : public IParticleObjectInfo
{
	I_DECLARE_DYNCREATE(IParticleObjectInfo_Billboard);
public:
	IParticleObjectInfo_Billboard(void);
	virtual ~IParticleObjectInfo_Billboard(void);

	virtual void		CreateObject();
	virtual void		SetObject(IGameObject * pGameObject);

	IGwBillboard	*	GetBillboard();

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

public:
	string				m_strMediaPath;
	string				m_strFileName;

	IGwBillboard	*	m_pBillboard;
};

//////////////////////////////////////////////////////////////////////////
// Mesh

class IParticleObjectInfo_Mesh : public IParticleObjectInfo
{
	I_DECLARE_DYNCREATE(IParticleObjectInfo_Mesh);
public:
	IParticleObjectInfo_Mesh(void);
	virtual ~IParticleObjectInfo_Mesh(void);

	virtual void		CreateObject();
	virtual void		SetObject(IGameObject * pGameObject);

	IGwStaticObject *	GetMeshObject();

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

public:
	string				m_strMediaPath;
	string				m_strFileName;

	vector<ITextureFileInfo> m_MeshTextureInfoList;

	IGwStaticObject	*	m_pMeshObject;
};

//////////////////////////////////////////////////////////////////////////
// SkinMesh

class IParticleObjectInfo_SkinMesh : public IParticleObjectInfo_Mesh
{
	I_DECLARE_DYNCREATE(IParticleObjectInfo_SkinMesh);
public:
	IParticleObjectInfo_SkinMesh(void);
	virtual ~IParticleObjectInfo_SkinMesh(void);

	virtual void		CreateObject();
	virtual void		SetObject(IGameObject * pGameObject);

	IGwStaticObject *	GetBoneObject();
	IGwAnimSequence	*	GetAnimSequence();

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

public:
	string				m_strBoneMediaPath;
	string				m_strBoneFileName;
	string				m_strAniMediaPath;
	string				m_strAnimFileName;

	IGwStaticObject	*	m_pBoneObject;
	IGwAnimSequence	*	m_pAnimSequence;
};