#pragma once

//////////////////////////////////////////////////////////////////////////
// IParticleInfo

class IParticleInfo : public IGwRefObject, public ITypeObject
{
	I_DECLARE_DYNCREATE(IParticleInfo);
public:
	IParticleInfo(void);
	virtual ~IParticleInfo(void);

	virtual void		CreateObject()								{ }
	virtual void		SetObject(IGameObject * pGameObject)		{ }
	virtual void		SetRenderState(eEsRenderState eRenderState)	{ m_eRenderState = eRenderState; }

	virtual void		LoadFile(BYTE * pData)		{ }
	virtual void		SaveFile(IFile & kFile)		{ }

public:
	string				m_strGameObjectName;
	eEsRenderState		m_eRenderState;
};

//////////////////////////////////////////////////////////////////////////
// Billboard

class IParticleInfo_Billboard : public IParticleInfo
{
	I_DECLARE_DYNCREATE(IParticleInfo_Billboard);
public:
	IParticleInfo_Billboard(void);
	virtual ~IParticleInfo_Billboard(void);

	virtual void		CreateObject();
	virtual void		SetObject(IGameObject * pGameObject);

	IGwBillboard	*	GetBillboard();

	virtual void		LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

public:
	string				m_strMediaPath;
	string				m_strFileName;

	IGwBillboard	*	m_pBillboard;
};

//////////////////////////////////////////////////////////////////////////
// Mesh

class IParticleInfo_Mesh : public IParticleInfo
{
	I_DECLARE_DYNCREATE(IParticleInfo_Mesh);
public:
	IParticleInfo_Mesh(void);
	virtual ~IParticleInfo_Mesh(void);

	virtual void		CreateObject();
	virtual void		SetObject(IGameObject * pGameObject);

	IGwStaticObject *	GetMeshObject();

	virtual void		LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

public:
	string				m_strMediaPath;
	string				m_strFileName;

	vector<ITextureFileInfo> m_MeshTextureInfoList;

	IGwStaticObject	*	m_pMeshObject;
};

//////////////////////////////////////////////////////////////////////////
// SkinMesh

class IParticleInfo_SkinMesh : public IParticleInfo_Mesh
{
	I_DECLARE_DYNCREATE(IParticleInfo_SkinMesh);
public:
	IParticleInfo_SkinMesh(void);
	virtual ~IParticleInfo_SkinMesh(void);

	virtual void		CreateObject();
	virtual void		SetObject(IGameObject * pGameObject);

	IGwStaticObject *	GetBoneObject();
	IGwAnimSequence	*	GetAnimSequence();

	virtual void		LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

public:
	string				m_strBoneMediaPath;
	string				m_strBoneFileName;
	string				m_strAniMediaPath;
	string				m_strAnimFileName;

	IGwStaticObject	*	m_pBoneObject;
	IGwAnimSequence	*	m_pAnimSequence;
};
