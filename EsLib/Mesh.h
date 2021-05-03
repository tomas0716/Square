#pragma once

class IMesh : public IGameObject
{
	I_DECLARE_DYNCREATE(IMesh);
public:
	IMesh();
	IMesh(const char * pName);
	IMesh(const char * pName, IGwStaticObject * pStaticObject, IFileInfo & kFileInfo, vector<ITextureFileInfo> & MeshTextureInfoList);
	virtual ~IMesh();

	virtual	void		Update(float fDeltaTime);
	virtual void		Render(float fDeltaTime);

	virtual void		SetMeshObject(IGwStaticObject * pStaticObject, IFileInfo & kFileInfo, vector<ITextureFileInfo> & MeshTextureInfoList);

	vector<ITextureFileInfo> * GetTextureFileInfoList();
	virtual void		SetTextureFileInfo(ITextureFileInfo * pTextureFileInfo);

	IGwScnObject	*	GetScnObject() { return m_pScnObject; }

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

protected:

	IGwScnObject				*	m_pScnObject;
	IGwStaticObject				*	m_pMeshObject;
	IFileInfo						m_FileInfo_Mesh;

	vector<ITextureFileInfo>		m_MeshTextureInfoList;
};