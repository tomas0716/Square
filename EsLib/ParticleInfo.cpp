#include "StdAfx.h"
#include "ParticleInfo.h"

//////////////////////////////////////////////////////////////////////////
// IParticleInfo

I_IMPLEMENT_DYNCREATE(IParticleInfo, ISingletonObject::GetTypeModul());

IParticleInfo::IParticleInfo(void) : m_eRenderState(eEsRenderState_AlphaBlending)
{
}

IParticleInfo::~IParticleInfo(void)
{
}

//////////////////////////////////////////////////////////////////////////
// Billboard

I_IMPLEMENT_DYNCREATE(IParticleInfo_Billboard, ISingletonObject::GetTypeModul());

IParticleInfo_Billboard::IParticleInfo_Billboard(void) : m_pBillboard(NULL)
{
	m_strGameObjectName = "IBillboard";
	m_pBillboard = Gw::GetBillboardMgr()->CreateBillboard();

	m_strMediaPath = "MediaPath\\Game2D\\Particle\\";
	m_strFileName = "heart_particle.png";
}

IParticleInfo_Billboard::~IParticleInfo_Billboard(void)
{
	m_pBillboard->Release();
}

void				IParticleInfo_Billboard::CreateObject()
{
	IGwBillboardTex	* pTex = IResourceHelper::Create_Billboard(m_strMediaPath, m_strFileName);
	m_pBillboard->SetBillboardTex(pTex); pTex->Release();

	switch(m_eRenderState)
	{
	case eEsRenderState_AlphaBlending:		// Src : SrcAlpha,	Dest : InvScrAlpha
		{
			m_pBillboard->SetSrcBlend(D3DBLEND_SRCALPHA);
			m_pBillboard->SetDestBlend(D3DBLEND_INVSRCALPHA);
		}
		break;
	case eEsRenderState_Addtive:			// Src : One,		Dest : One
		{
			m_pBillboard->SetSrcBlend(D3DBLEND_SRCALPHA);
			m_pBillboard->SetDestBlend(D3DBLEND_ONE);
		}
		break;
	case eEsRenderState_SoftAddtive:		// Src : One,		Dest : InvDestColor
		{
			m_pBillboard->SetSrcBlend(D3DBLEND_ONE);
			m_pBillboard->SetDestBlend(D3DBLEND_INVDESTCOLOR);
		}
		break;
	case eEsRenderState_Multiplicative:		// Src : DestColor, Dest : Zero
		{
			m_pBillboard->SetSrcBlend(D3DBLEND_DESTCOLOR);
			m_pBillboard->SetDestBlend(D3DBLEND_ZERO);
		}
		break;
	case eEsRenderState_2xMultiplicative:	// Src : DestColor,	Dest : SrcColor
		{
			m_pBillboard->SetSrcBlend(D3DBLEND_DESTCOLOR);
			m_pBillboard->SetDestBlend(D3DBLEND_SRCCOLOR);
		}
		break;
	}
}

void				IParticleInfo_Billboard::SetObject(IGameObject * pGameObject)
{
	IBillboard * pBillboard = dynamic_cast<IBillboard*>(pGameObject);

	if( pBillboard != NULL )
	{
		pBillboard->SetBillboard(m_pBillboard);
	}
}

IGwBillboard	*	IParticleInfo_Billboard::GetBillboard()
{
	return m_pBillboard;
}

void				IParticleInfo_Billboard::LoadFile(BYTE * pData)
{
	char szPath[128]		= { 0, };
	char szFileName[128]	= { 0, };

	memcpy((void*)szPath, pData, sizeof(char) * 128);		 pData += sizeof(char) * 128;
	memcpy((void*)szFileName, pData, sizeof(char) * 128);	 pData += sizeof(char) * 128;

	m_strMediaPath = szPath;
	m_strFileName = szFileName;
}

void				IParticleInfo_Billboard::SaveFile(IFile & kFile)
{
	kFile.WriteFile((void*)m_strMediaPath.c_str(), sizeof(char) * 128);
	kFile.WriteFile((void*)m_strFileName.c_str(), sizeof(char) * 128);
}

//////////////////////////////////////////////////////////////////////////
// Mesh

I_IMPLEMENT_DYNCREATE(IParticleInfo_Mesh, ISingletonObject::GetTypeModul());

IParticleInfo_Mesh::IParticleInfo_Mesh(void) : m_pMeshObject(NULL)
{
	m_strGameObjectName = "IMesh";
	m_pMeshObject = Gw::GetScnMgr()->CreateStaticObject();
}

IParticleInfo_Mesh::~IParticleInfo_Mesh(void)
{
	m_pMeshObject->Release();
}

void				IParticleInfo_Mesh::CreateObject()
{
	IGwStaticObject * pStaticObject = NULL;
	if( m_MeshTextureInfoList.empty() == true )
	{
		pStaticObject = IResourceHelper::Create_Init_StaticObject(m_strMediaPath, m_strFileName, m_MeshTextureInfoList);
	}
	else 
	{
		pStaticObject = IResourceHelper::Create_StaticObject(m_strMediaPath, m_strFileName, &m_MeshTextureInfoList);
	}

	if( pStaticObject != NULL )
	{
		m_pMeshObject->Release();
		m_pMeshObject = pStaticObject;
	}
}

void				IParticleInfo_Mesh::SetObject(IGameObject * pGameObject)
{
	IMesh * pMesh = dynamic_cast<IMesh*>(pGameObject);

	if( pMesh != NULL )
	{
		pMesh->SetMeshObject(m_pMeshObject);
	}
}

IGwStaticObject *	IParticleInfo_Mesh::GetMeshObject()
{
	return m_pMeshObject;
}

void				IParticleInfo_Mesh::LoadFile(BYTE * pData)
{
	char szPath[128]		= { 0, };
	char szFileName[128]	= { 0, };

	memcpy((void*)szPath, pData, sizeof(char) * 128);		 pData += sizeof(char) * 128;
	memcpy((void*)szFileName, pData, sizeof(char) * 128);	 pData += sizeof(char) * 128;

	m_strMediaPath = szPath;
	m_strFileName = szFileName;

	int nCount = 0;
	memcpy(&nCount, pData, sizeof(int));	 pData += sizeof(int);

	for( int i = 0; i < nCount; ++i )
	{
		ITextureFileInfo kTextureFileInfo;
		kTextureFileInfo.LoadFile(pData);
		m_MeshTextureInfoList.push_back(kTextureFileInfo);
	}
}

void				IParticleInfo_Mesh::SaveFile(IFile & kFile)
{
	kFile.WriteFile((void*)m_strMediaPath.c_str(), sizeof(char) * 128);
	kFile.WriteFile((void*)m_strFileName.c_str(), sizeof(char) * 128);

	int nCount = m_MeshTextureInfoList.size();
	kFile.WriteFile(&nCount, sizeof(int));

	for( int i = 0; i < nCount; ++i )
	{
		m_MeshTextureInfoList[i].SaveFile(kFile);
	}
}

//////////////////////////////////////////////////////////////////////////
// SkinMesh

I_IMPLEMENT_DYNCREATE(IParticleInfo_SkinMesh, ISingletonObject::GetTypeModul());

IParticleInfo_SkinMesh::IParticleInfo_SkinMesh(void) : m_pBoneObject(NULL), m_pAnimSequence(NULL)
{
	m_strGameObjectName = "ISkinMesh";

	m_pBoneObject = Gw::GetScnMgr()->CreateStaticObject();
	m_pAnimSequence = Gw::GetAnimMgr()->GetAnimSequence_Null();
}

IParticleInfo_SkinMesh::~IParticleInfo_SkinMesh(void)
{
	m_pBoneObject->Release();
	m_pAnimSequence->Release();
}

void				IParticleInfo_SkinMesh::CreateObject()
{
	IParticleInfo_Mesh::CreateObject();

	IGwStaticObject * pStaticObject = IResourceHelper::Create_StaticObject(m_strBoneMediaPath, m_strBoneFileName, &m_MeshTextureInfoList);
	if( pStaticObject )
	{
		m_pBoneObject->Release();
		m_pBoneObject = pStaticObject;
	}

	IGwAnimSequence * pAnimSequence = IResourceHelper::Create_AnimSequence(m_strBoneMediaPath, m_strBoneFileName);
	if( pAnimSequence )
	{
		m_pAnimSequence->Release();
		m_pAnimSequence = pAnimSequence;
	}
}

void				IParticleInfo_SkinMesh::SetObject(IGameObject * pGameObject)
{
	ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(pGameObject);

	if( pSkinMesh != NULL )
	{
		pSkinMesh->SetMeshObject(m_pMeshObject);
		pSkinMesh->SetBoneObject(m_pBoneObject);
		pSkinMesh->SetAnimSequence(m_pAnimSequence);
	}
}

IGwStaticObject *	IParticleInfo_SkinMesh::GetBoneObject()
{
	return m_pBoneObject;
}

IGwAnimSequence	*	IParticleInfo_SkinMesh::GetAnimSequence()
{
	return m_pAnimSequence;
}

void				IParticleInfo_SkinMesh::LoadFile(BYTE * pData)
{
	IParticleInfo_Mesh::LoadFile(pData);

	char szBoneMediaPath[128]	= { 0, };
	char szBoneFileName[128]		= { 0, };

	memcpy((void*)szBoneMediaPath, pData, sizeof(char) * 128);	 pData += sizeof(char) * 128;
	memcpy((void*)szBoneFileName, pData, sizeof(char) * 128);	 pData += sizeof(char) * 128;

	m_strBoneMediaPath = szBoneMediaPath;
	m_strBoneFileName = szBoneFileName;

	char szAniMediaPath[128]	= { 0, };
	char szAniFileName[128]		= { 0, };

	memcpy((void*)szAniMediaPath, pData, sizeof(char) * 128);	 pData += sizeof(char) * 128;
	memcpy((void*)szAniFileName, pData, sizeof(char) * 128);	 pData += sizeof(char) * 128;

	m_strAniMediaPath = szAniMediaPath;
	m_strAnimFileName = szAniFileName;
}

void				IParticleInfo_SkinMesh::SaveFile(IFile & kFile)
{
	IParticleInfo_Mesh::SaveFile(kFile);

	kFile.WriteFile((void*)m_strBoneMediaPath.c_str(), sizeof(char) * 128);
	kFile.WriteFile((void*)m_strBoneFileName.c_str(), sizeof(char) * 128);

	kFile.WriteFile((void*)m_strAniMediaPath.c_str(), sizeof(char) * 128);
	kFile.WriteFile((void*)m_strAnimFileName.c_str(), sizeof(char) * 128);
}

