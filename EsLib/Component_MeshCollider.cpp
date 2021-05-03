#include "stdafx.h"
#include "Component_MeshCollider.h"

I_IMPLEMENT_DYNCREATE(IComponent_MeshCollider, ISingletonObject::GetTypeModule())

IComponent_MeshCollider::IComponent_MeshCollider()
: IComponent(NULL), m_pMeshObject(NULL), m_pOBB(NULL), m_IsCameraRayCast(true)
{
}

IComponent_MeshCollider::IComponent_MeshCollider(IGameObject * pParent)
: IComponent(pParent), m_pMeshObject(NULL), m_pOBB(NULL), m_IsCameraRayCast(false)
{
}

IComponent_MeshCollider::~IComponent_MeshCollider()
{
	if( m_pMeshObject != NULL ) 
		m_pMeshObject->Release();
}

void			IComponent_MeshCollider::Update(float fDeltaTime)
{
	if( m_pMeshObject != NULL )
		m_pMeshObject->Update(fDeltaTime);
}

void			IComponent_MeshCollider::Render(float fDeltaTime)
{
}

void			IComponent_MeshCollider::SetMeshObject(IGwStaticObject * pStaticObject, IFileInfo & kFileInfo)
{
	if( m_pMeshObject != NULL ) m_pMeshObject->Release();
	m_pMeshObject = pStaticObject;

	if( m_pMeshObject != NULL ) m_pMeshObject->AddRef();

	m_FileInfo_Mesh = kFileInfo;
}

void			IComponent_MeshCollider::SetCameraRayCast(bool IsRayCast)
{
	m_IsCameraRayCast = IsRayCast;
}

bool			IComponent_MeshCollider::IsCameraRayCast()
{
	return m_IsCameraRayCast;
}

void			IComponent_MeshCollider::CalcAABB()
{
	if( m_pParentGameObject != NULL && m_pMeshObject != NULL )
	{
		GwMatA matWorld = m_pParentGameObject->GetWorldMat();
		m_pMeshObject->SetWorldMatrix(matWorld);
		m_pMeshObject->Update(0);
		m_pMeshObject->CalcAABB(&m_AABB, TRUE);
	}
}

GwAABB		*	IComponent_MeshCollider::GetAABB()
{
	return &m_AABB;
}

const GwOBB	*	IComponent_MeshCollider::GetOBB()
{
	if( m_pOBB != NULL ) 
		return m_pOBB;

	if( m_pParentGameObject != NULL && m_pMeshObject != NULL )
	{
		GwMatA matWorld = m_pParentGameObject->GetWorldMat();
		m_pMeshObject->SetWorldMatrix(matWorld);
		m_pMeshObject->Update(0);

		vector<IGwRndNode*> MeshList;
		m_pMeshObject->FindAllTreeNodes(MeshList, GWRNDNODE_MESH);

		if( MeshList.empty() == false )
		{
			MeshList[0]->GetOBBWorld(m_pOBB); 

			return m_pOBB;
		}
	}

	return NULL;
}

BYTE		*	IComponent_MeshCollider::LoadFile(BYTE * pData)
{
	pData = IComponent::LoadFile(pData);

	pData = m_FileInfo_Mesh.LoadFile(pData);
	memcpy(&m_IsCameraRayCast, pData, sizeof(bool));	pData += sizeof(bool);

	IGwStaticObject * pMeshObject = IResourceHelper::CreateStaticObject(m_FileInfo_Mesh.m_strPath.c_str(), m_FileInfo_Mesh.m_strPath.c_str(), m_FileInfo_Mesh.m_strFileName.c_str());
	SetMeshObject(pMeshObject, m_FileInfo_Mesh);
	if( pMeshObject != NULL )pMeshObject->Release();

	return pData;
}

void			IComponent_MeshCollider::SaveFile(IFile & kFile)
{
	IComponent::SaveFile(kFile);

	m_FileInfo_Mesh.SaveFile(kFile);
	kFile.WriteFile(&m_IsCameraRayCast, sizeof(bool));
}
