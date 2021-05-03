#include "StdAfx.h"
#include "SkinMesh.h"

I_IMPLEMENT_DYNCREATE(ISkinMesh, ISingletonObject::GetTypeModule())

ISkinMesh::ISkinMesh() : m_nAnimSequenceIndex(0)
{
	m_pScnObject = Gw::GetScnMgr()->CreateScnObject();
	m_pMeshObject = Gw::GetScnMgr()->GetStaticObject_Null()->Clone();
	m_pBoneObject = Gw::GetScnMgr()->GetStaticObject_Null()->Clone();
	m_pScnObject->SetStaticObject(eMeshObject,m_pMeshObject);
	m_pScnObject->SetStaticObject(eBoneObject,m_pBoneObject);
}

ISkinMesh::ISkinMesh(const char * pName)
: IGameObject(pName), m_nAnimSequenceIndex(0)
{
	m_pScnObject = Gw::GetScnMgr()->CreateScnObject();
	m_pMeshObject = Gw::GetScnMgr()->GetStaticObject_Null()->Clone();
	m_pBoneObject = Gw::GetScnMgr()->GetStaticObject_Null()->Clone();
	m_pScnObject->SetStaticObject(eMeshObject,m_pMeshObject);
	m_pScnObject->SetStaticObject(eBoneObject,m_pBoneObject);
}

ISkinMesh::ISkinMesh(const char * pName, IGwStaticObject * pStaticObject, IFileInfo & kFileInfo, vector<ITextureFileInfo> & MeshTextureInfoList)
: IGameObject(pName), m_nAnimSequenceIndex(0), m_MeshTextureInfoList(MeshTextureInfoList)
{
	m_pScnObject = Gw::GetScnMgr()->CreateScnObject();
	m_pMeshObject = pStaticObject->Clone();
	m_FileInfo_Mesh = kFileInfo;
	m_MeshTextureInfoList = MeshTextureInfoList;

	m_pBoneObject = Gw::GetScnMgr()->GetStaticObject_Null()->Clone();
	
	m_pScnObject->SetStaticObject(eMeshObject,m_pMeshObject);	
	m_pScnObject->SetStaticObject(eBoneObject,m_pBoneObject);
}

ISkinMesh::~ISkinMesh()
{
	m_pScnObject->Release();
	m_pMeshObject->Release();
	SAFE_RELEASE(m_pBoneObject);

	map<int,IGwAnimSequence*>::iterator itr = m_AnimSequenceTable.begin();

	for( int i = 0; itr != m_AnimSequenceTable.end(); ++itr, ++i )
	{
		itr->second->Release();
	}

	m_AnimSequenceTable.clear();
}

void		ISkinMesh::Update(float fDeltaTime)
{
	m_pScnObject->SetWorldMatrix(GetWorldMat());
	m_pScnObject->Update(fDeltaTime);

	IGameObject::Update(fDeltaTime);
}

void		ISkinMesh::Render(float fDeltaTime)
{
	if( IsVisible() == true && IsVisible_ClientControl() == true )
	{
		Gw::GetRenderer()->AddScnObject(m_pScnObject);
	}

	IGameObject::Render(fDeltaTime);
}

void		ISkinMesh::Render(float fDeltaTime, IGwViewFrustumOct * pFrustum)
{
	if( IsVisible() == true && IsVisible_ClientControl() == true )
	{
		GwMatA matWorld = *m_pScnObject->GetWorldMatrix();

		map<IGwRndNode*, GwAABB>::iterator itr = m_RndNodeAABBList.begin();

		for( ; itr != m_RndNodeAABBList.end(); ++itr )
		{
			if( pFrustum->CheckIfInFrustum(itr->second) == TRUE )
			{
				Gw::GetRenderer()->AddMesh((IGwBaseMesh*)(itr->first));
			}
		}
	}

	IGameObject::Render(fDeltaTime, pFrustum);
}

void		ISkinMesh::SetMeshObject(IGwStaticObject * pStaticObject, IFileInfo & kFileInfo, vector<ITextureFileInfo>	& MeshTextureInfoList)
{
	m_pMeshObject->Release();
	m_pMeshObject = pStaticObject->Clone();
	m_pScnObject->SetStaticObject(eMeshObject,m_pMeshObject);

	m_FileInfo_Mesh = kFileInfo;
	m_MeshTextureInfoList = MeshTextureInfoList;

	m_MeshTextureInfoList = MeshTextureInfoList;
}

void		ISkinMesh::SetBoneObject(IGwStaticObject * pStaticObject, IFileInfo & kFileInfo)
{
	m_pBoneObject->Release();

	if( pStaticObject != NULL )
	{
		m_pBoneObject = pStaticObject->Clone();
		m_pScnObject->SetStaticObject(eBoneObject,m_pBoneObject);
	}
	else 
	{
		m_pBoneObject = Gw::GetScnMgr()->GetStaticObject_Null()->Clone();
		m_pScnObject->SetStaticObject(eBoneObject,m_pBoneObject);
	}

	m_FileInfo_Bone = kFileInfo;
}

void		ISkinMesh::AddAnimSequence(IGwAnimSequence * pAnimSequence, IFileInfo & kFileInfo)
{
	if( pAnimSequence == NULL )
		return;

	m_AnimSequenceTable.insert(map<int,IGwAnimSequence*>::value_type(m_nAnimSequenceIndex, pAnimSequence));
	pAnimSequence->AddRef();

	m_pScnObject->SetAnimSequence(m_nAnimSequenceIndex, pAnimSequence);

	m_FileInfoTable_Anim.insert(map<int,IFileInfo>::value_type(m_nAnimSequenceIndex, kFileInfo));

	++m_nAnimSequenceIndex;
}

bool		ISkinMesh::ChangeAnimSequence(int nIndex, IGwAnimSequence * pAnimSequence, IFileInfo & kFileInfo)
{
	map<int,IGwAnimSequence*>::iterator itr = m_AnimSequenceTable.find(nIndex);

	if( itr != m_AnimSequenceTable.end() )
	{
		itr->second->Release();
		itr->second = pAnimSequence;
		itr->second->AddRef();

		m_pScnObject->SetAnimSequence(nIndex, pAnimSequence);

		m_FileInfoTable_Anim[nIndex] = kFileInfo;

		return true;
	}

	return false;
}

void		ISkinMesh::RemoveAnimSequence(int nIndex)
{
	map<int,IGwAnimSequence*>::iterator itr = m_AnimSequenceTable.begin();

	for( int i = 0; itr != m_AnimSequenceTable.end(); ++itr, ++i )
	{
		m_pScnObject->SetAnimSequence(i, Gw::GetAnimMgr()->GetAnimSequence_Null());
	}

	itr = m_AnimSequenceTable.find(nIndex);

	if( itr != m_AnimSequenceTable.end() )
	{
		--m_nAnimSequenceIndex;
		itr->second->Release();
		m_AnimSequenceTable.erase(itr);
	}

	itr = m_AnimSequenceTable.begin();

	for( int i = 0; itr != m_AnimSequenceTable.end(); ++itr, ++i )
	{
		m_pScnObject->SetAnimSequence(i, itr->second);
	}

	map<int,IFileInfo>::iterator itr_F = m_FileInfoTable_Anim.find(nIndex);

	if( itr_F != m_FileInfoTable_Anim.end() )
	{
		m_FileInfoTable_Anim.erase(itr_F);
	}
}

void		ISkinMesh::RemoveAnimSequenceAll()
{
	m_nAnimSequenceIndex = 0;

	map<int,IGwAnimSequence*>::iterator itr = m_AnimSequenceTable.begin();

	for( int i = 0; itr != m_AnimSequenceTable.end(); ++itr, ++i )
	{
		m_pScnObject->SetAnimSequence(i, Gw::GetAnimMgr()->GetAnimSequence_Null());
		itr->second->Release();
	}

	m_AnimSequenceTable.clear();

	m_FileInfoTable_Anim.clear();
}

void		ISkinMesh::OnAnimPlay(float fTransitionTime, BOOL IsLoop)
{
	IGwAnimEvent*	pAnimEvent = Gw::GetAnimMgr()->CreateAnimEvent();
	pAnimEvent->SetTransitionTime(fTransitionTime);

	for( int i = 0; i < m_nAnimSequenceIndex; ++i )
	{
		m_pScnObject->SetAnimEvent(i, pAnimEvent);
		m_pScnObject->SetLoopEnable(IsLoop);
		m_pScnObject->SelectAnimation(i, i, i);
	}

	pAnimEvent->Release();
}

vector<ITextureFileInfo> * ISkinMesh::GetTextureFileInfoList()
{
	return &m_MeshTextureInfoList;
}

void		ISkinMesh::SetTextureFileInfo(ITextureFileInfo * pTextureFileInfo)
{
	IResourceHelper::SetStaticObjectTexture(m_pMeshObject, pTextureFileInfo);

	vector<ITextureFileInfo>::iterator itr = m_MeshTextureInfoList.begin();

	for( ; itr != m_MeshTextureInfoList.end(); ++itr )
	{
		ITextureFileInfo * pInfo = &(*itr);

		if( pInfo->m_nMeshIndex == pTextureFileInfo->m_nMeshIndex && 
			pInfo->m_nSubMtrlSetIndex == pTextureFileInfo->m_nSubMtrlSetIndex && 
			pInfo->m_nTextureIndex == pTextureFileInfo->m_nTextureIndex )
		{
			pInfo->m_strPath = pTextureFileInfo->m_strPath;
			pInfo->m_strFileName = pTextureFileInfo->m_strFileName;
		}
	}
}

map<int,IFileInfo>	*	ISkinMesh::GetAnimFileInfoTable()
{
	return &m_FileInfoTable_Anim;
}

void		ISkinMesh::CalcAABB()
{
	m_pScnObject->Update(0);

	vector<IGwRndNode*> MeshList;
	m_pMeshObject->FindAllTreeNodes(MeshList, GWRNDNODE_MESH);

	vector<IGwRndNode*>::iterator itr = MeshList.begin();

	for( ; itr != MeshList.end(); ++itr )
	{
		IGwRndNode * pRndNode = (*itr);
		if( pRndNode != NULL )
		{
			GwAABB aabb;
			pRndNode->CalcAABB(&aabb, FALSE);

			m_RndNodeAABBList.insert(map<IGwRndNode*, GwAABB>::value_type(pRndNode, aabb));
		}
	}

	IGameObject::CalcAABB();
}

BYTE	*	ISkinMesh::LoadFile(BYTE * pData)
{
	pData = IGameObject::LoadFile(pData);

	pData = m_FileInfo_Mesh.LoadFile(pData);

	m_MeshTextureInfoList.clear();

	int nNumTextureFileInfo = 0;
	memcpy(&nNumTextureFileInfo, pData, sizeof(int));	pData += sizeof(int);

	for( int i = 0; i < nNumTextureFileInfo ; ++i )
	{
		ITextureFileInfo kInfo;
		pData = kInfo.LoadFile(pData);
		m_MeshTextureInfoList.push_back (kInfo);
	}

	bool IsBone = false;
	memcpy(&IsBone, pData, sizeof(bool));	pData += sizeof(bool);

	if( IsBone == true )
	{
		pData = m_FileInfo_Bone.LoadFile(pData);
	}

	int nNumAnimSequence = 0;
	memcpy(&nNumAnimSequence, pData, sizeof(int));	pData += sizeof(int);

	for( int i = 0; i < nNumAnimSequence; ++i )
	{
		IFileInfo kFileInfo;
		pData = kFileInfo.LoadFile(pData);

		m_FileInfoTable_Anim.insert(map<int,IFileInfo>::value_type(i, kFileInfo));
	}

	// 1차 시연을 위해 텍스쳐는 바꾸더라도 무시한다.
	//IGwStaticObject * pMeshObject = IResourceHelper::CreateStaticObject(m_FileInfo_Mesh.m_strPath.c_str(), m_FileInfo_Mesh.m_strPath.c_str(), m_FileInfo_Mesh.m_strFileName.c_str());
	IGwStaticObject * pMeshObject = IResourceHelper::CreateStaticObject_AutoTexture(m_FileInfo_Mesh.m_strPath.c_str(), m_FileInfo_Mesh.m_strFileName.c_str(), m_MeshTextureInfoList);
	SetMeshObject(pMeshObject, m_FileInfo_Mesh, m_MeshTextureInfoList);
	pMeshObject->Release();
	nNumTextureFileInfo = (int)m_MeshTextureInfoList.size();
	for( int i = 0; i < nNumTextureFileInfo ; ++i )
	{
		ITextureFileInfo kInfo = m_MeshTextureInfoList[i];
		SetTextureFileInfo(&kInfo);
	}

	if( IsBone == true && m_FileInfo_Bone.m_strPath != "" && m_FileInfo_Bone.m_strFileName != "" )
	{
		IGwStaticObject * pBoneObject = IResourceHelper::CreateStaticObject(m_FileInfo_Bone.m_strPath.c_str(), m_FileInfo_Bone.m_strPath.c_str(), m_FileInfo_Bone.m_strFileName.c_str());
		SetBoneObject(pBoneObject, m_FileInfo_Bone);
		pBoneObject->Release();
	}

	map<int,IFileInfo>::iterator itr = m_FileInfoTable_Anim.begin();

	for( ; itr != m_FileInfoTable_Anim.end(); ++itr )
	{
		IGwAnimSequence	* pAnimSequence = IResourceHelper::Create_AnimSequence(itr->second.m_strPath.c_str(), itr->second.m_strFileName.c_str());
		AddAnimSequence(pAnimSequence, itr->second);	pAnimSequence->Release();
	}

	OnAnimPlay();

	Update(0);

	return pData;
}

void		ISkinMesh::SaveFile(IFile & kFile)
{
	IGameObject::SaveFile(kFile);

	m_FileInfo_Mesh.SaveFile(kFile);

	int nNumTextureFileInfo = (int)m_MeshTextureInfoList.size();
	kFile.WriteFile(&nNumTextureFileInfo, sizeof(int));

	for( int i = 0; i < nNumTextureFileInfo ; ++i )
	{
		ITextureFileInfo kInfo = m_MeshTextureInfoList[i];
		kInfo.SaveFile(kFile);
	}

	bool IsBone = m_pBoneObject == NULL ? false : true;
	kFile.WriteFile(&IsBone, sizeof(bool));
	if( IsBone == true )
	{
		m_FileInfo_Bone.SaveFile(kFile);
	}

	int nNumAnimSequence = (int)m_FileInfoTable_Anim.size();
	kFile.WriteFile(&nNumAnimSequence, sizeof(int));

	map<int,IFileInfo>::iterator itr = m_FileInfoTable_Anim.begin();

	for( ; itr != m_FileInfoTable_Anim.end(); ++itr )
	{
		itr->second.SaveFile(kFile);
	}
}
