#pragma once

class ISkinMesh : public IGameObject
{
	I_DECLARE_DYNCREATE(ISkinMesh);
public:
	ISkinMesh();
	ISkinMesh(const char * pName);
	ISkinMesh(const char * pName, IGwStaticObject * pStaticObject, IFileInfo & kFileInfo, vector<ITextureFileInfo> & MeshTextureInfoList);
	virtual ~ISkinMesh();

	virtual	void		Update(float fDeltaTime);
	virtual void		Render(float fDeltaTime);
	virtual	void		Render(float fDeltaTime, IGwViewFrustumOct * pFrustum);

	virtual void		SetMeshObject(IGwStaticObject * pStaticObject, IFileInfo & kFileInfo, vector<ITextureFileInfo> & MeshTextureInfoList);
	virtual void		SetBoneObject(IGwStaticObject * pStaticObject, IFileInfo & kFileInfo);

	virtual void		AddAnimSequence(IGwAnimSequence * pAnimSequence, IFileInfo & kFileInfo);
	virtual bool		ChangeAnimSequence(int nIndex, IGwAnimSequence * pAnimSequence, IFileInfo & kFileInfo);
	virtual void		RemoveAnimSequence(int nIndex);
	virtual void		RemoveAnimSequenceAll();

	vector<ITextureFileInfo> * GetTextureFileInfoList();
	virtual void		SetTextureFileInfo(ITextureFileInfo * pTextureFileInfo);

	map<int,IFileInfo>	*	GetAnimFileInfoTable();

	virtual void		OnAnimPlay(float fTransitionTime = 0.0f, BOOL IsLoop = TRUE);

	IGwScnObject	*	GetScnObject() { return m_pScnObject; }

	virtual void		CalcAABB();

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

protected:

	void				ResetAnimSequence();
	enum
	{
		eMeshObject,
		eBoneObject,
	};

	IGwScnObject				*	m_pScnObject;
	IGwStaticObject				*	m_pMeshObject;
	IFileInfo						m_FileInfo_Mesh;
	vector<ITextureFileInfo>		m_MeshTextureInfoList;

	IGwStaticObject 			*	m_pBoneObject;
	IFileInfo						m_FileInfo_Bone;

	int								m_nAnimSequenceIndex;
	map<int,IGwAnimSequence*>		m_AnimSequenceTable;
	map<int,IFileInfo>				m_FileInfoTable_Anim;

	map<IGwRndNode*, GwAABB>		m_RndNodeAABBList;
};