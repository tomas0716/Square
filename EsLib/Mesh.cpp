#include "stdafx.h"
#include "Mesh.h"

I_IMPLEMENT_DYNCREATE(IMesh, ISingletonObject::GetTypeModul())

IMesh::IMesh()
{
	m_pScnObject = Gw::GetScnMgr()->CreateScnObject();
	m_pMeshObject = Gw::GetScnMgr()->CreateStaticObject();
	m_pScnObject->AddStaticObject(m_pMeshObject);
}

IMesh::IMesh(const char * pName)
: IGameObject(pName)
{
	m_pScnObject = Gw::GetScnMgr()->CreateScnObject();
	m_pMeshObject = Gw::GetScnMgr()->CreateStaticObject();
	m_pScnObject->AddStaticObject(m_pMeshObject);
}

IMesh::IMesh(const char * pName, IGwStaticObject * pStaticObject, IFileInfo & kFileInfo, vector<ITextureFileInfo> & MeshTextureInfoList)
: IGameObject(pName), m_MeshTextureInfoList(MeshTextureInfoList)
{
	m_pScnObject = Gw::GetScnMgr()->CreateScnObject();
	m_pMeshObject = pStaticObject->Clone();
	m_pScnObject->AddStaticObject(m_pMeshObject);

	m_MeshTextureInfoList = MeshTextureInfoList;
	m_FileInfo_Mesh = kFileInfo;
}

IMesh::~IMesh()
{	
	m_pScnObject->Release();
	m_pMeshObject->Release();
}

void		IMesh::Update(float fDeltaTime)
{
	if( m_Color == GwColor(1,1,1,1))	m_pScnObject->EnableTFactorColor(FALSE);
	else								m_pScnObject->EnableTFactorColor(TRUE, &m_Color, D3DTOP_MODULATE);

	m_pScnObject->SetWorldMatrix(GetWorldMat());
	m_pScnObject->Update(fDeltaTime);

	IGameObject::Update(fDeltaTime);
}

void		IMesh::Render(float fDeltaTime)
{
	if( IsVisible() == true )
	{
		Gw::GetRenderer()->AddScnObject(m_pScnObject);
	}

	IGameObject::Render(fDeltaTime);
}

void		IMesh::SetMeshObject(IGwStaticObject * pStaticObject, IFileInfo & kFileInfo, vector<ITextureFileInfo> & MeshTextureInfoList)
{
	m_pMeshObject->Release();
	m_pMeshObject = pStaticObject->Clone();
	m_pScnObject->SetStaticObject(0,m_pMeshObject);
}

vector<ITextureFileInfo> * IMesh::GetTextureFileInfoList()
{
	return &m_MeshTextureInfoList;
}

void		IMesh::SetTextureFileInfo(ITextureFileInfo * pTextureFileInfo)
{
	IResourceHelper::SetStaticObjectTexture(m_pMeshObject, pTextureFileInfo);
}

BYTE	*	IMesh::LoadFile(BYTE * pData)
{
	pData = IGameObject::LoadFile(pData);

	pData = m_FileInfo_Mesh.LoadFile(pData);

	int nNumTextureFileInfo = 0;
	memcpy(&nNumTextureFileInfo, pData, sizeof(int));	pData += sizeof(int);

	for( int i = 0; i < nNumTextureFileInfo; ++i )
	{
		ITextureFileInfo kFileInfo;
		pData = kFileInfo.LoadFile(pData);

		m_MeshTextureInfoList.push_back(kFileInfo);
	}

	IGwStaticObject * pMeshObject = IResourceHelper::CreateStaticObject(m_FileInfo_Mesh.m_strPath.c_str(), m_FileInfo_Mesh.m_strPath.c_str(), m_FileInfo_Mesh.m_strFileName.c_str());
	SetMeshObject(pMeshObject, m_FileInfo_Mesh, m_MeshTextureInfoList);
	pMeshObject->Release();
	nNumTextureFileInfo = (int)m_MeshTextureInfoList.size();
	for( int i = 0; i < nNumTextureFileInfo ; ++i )
	{
		ITextureFileInfo kInfo = m_MeshTextureInfoList[i];
		SetTextureFileInfo(&kInfo);
	}

	return pData;
}

void		IMesh::SaveFile(IFile & kFile)
{
	IGameObject::SaveFile(kFile);

	m_FileInfo_Mesh.SaveFile(kFile);

	int nNumTextureFileInfo = (int)m_MeshTextureInfoList.size();
	kFile.WriteFile(&nNumTextureFileInfo, sizeof(int));

	vector<ITextureFileInfo>::iterator itr = m_MeshTextureInfoList.begin();

	for( ; itr != m_MeshTextureInfoList.end(); ++itr )
	{
		(*itr).SaveFile(kFile);
	}	
}
