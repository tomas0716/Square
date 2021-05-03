#include "StdAfx.h"
#include "PickingObjectWireFrameRender.h"

struct sBuffer
{
	GwVector	*	pVertices;
	int				nNumVertices;
	WORD		*	pIndices;
	int				nNumIndices;

	vector<sBuffer*>	GetBufferList()
	{
		vector<sBuffer*> sBufferList;

		int nSplit = nNumIndices / 6000;
		int nRename = nNumIndices % 6000;

		for( int i = 0; i < nSplit; ++i )
		{
			sBuffer * pBuffer = new sBuffer;

			pBuffer->nNumVertices = 6000;
			pBuffer->nNumIndices = 6000;

			pBuffer->pVertices = new GwVector[6000];
			pBuffer->pIndices = new WORD[6000];

			for( int j = 0; j < 6000; ++j )
			{
				pBuffer->pVertices[j] = pVertices[pIndices[i * 6000 + j]];
				pBuffer->pIndices[j] = j;
			}

			sBufferList.push_back(pBuffer);
		}

		if( nRename != 0 )
		{
			sBuffer * pBuffer = new sBuffer;

			pBuffer->nNumVertices = nRename;
			pBuffer->nNumIndices = nRename;

			pBuffer->pVertices = new GwVector[nRename];
			pBuffer->pIndices = new WORD[nRename];

			for( int j = 0; j < nRename; ++j )
			{
				pBuffer->pVertices[j] = pVertices[pIndices[nSplit * 6000 + j]];
				pBuffer->pIndices[j] = j;
			}

			sBufferList.push_back(pBuffer);
		}

		return sBufferList;
	}
};

CPickingObjectWireFrameRender::CPickingObjectWireFrameRender(void) : m_IsUpdate(false), m_IsRender(true)
{
	m_pSelGameObject = IGameObjectMgr::GetInstance()->GetEmptyGameObject();
	m_pSelGameObject->AddRef();
}

CPickingObjectWireFrameRender::~CPickingObjectWireFrameRender(void)
{
	m_pSelGameObject->Release();

	ClearAllDynPolygon();
}

void	CPickingObjectWireFrameRender::ClearAllDynPolygon()
{
	vector<IGwDynPolygon*>::iterator itr = m_DynPolygonList.begin();

	for( ; itr != m_DynPolygonList.end(); ++itr )
	{
		(*itr)->Release();
	}

	m_DynPolygonList.clear();
}

void	CPickingObjectWireFrameRender::Update(float fDeltaTime)
{
	if( m_IsUpdate == true )
	{
		m_IsUpdate = false;

		ClearAllDynPolygon();

		if( m_pSelGameObject != IGameObjectMgr::GetInstance()->GetEmptyGameObject() && m_pSelGameObject->IsVisible() == true && m_IsRender == true )
		{
			vector<sBuffer*>	BufferList;

			int nNumChild = 0;
			string strClassName = IHashObject::GetInstance()->GetHashString(m_pSelGameObject->GetClassHashCode());
			if( strcmp(strClassName.c_str(), "ISkinMesh") == 0 )
			{
				ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(m_pSelGameObject);

				if( pSkinMesh != NULL )
					nNumChild = pSkinMesh->GetScnObject()->GetNumStaticObject();
			}

			for( int i = 0; i < nNumChild; ++i )
			{
				IGwStaticObject * pStaticObject = NULL;

				if( strcmp(strClassName.c_str(), "ISkinMesh") == 0 )
				{
					ISkinMesh * pSkinMesh = dynamic_cast<ISkinMesh*>(m_pSelGameObject);

					if( pSkinMesh != NULL )
						pStaticObject = pSkinMesh->GetScnObject()->GetStaticObject(i);
				}

				if( pStaticObject != NULL )
				{
					vector<IGwRndNode*> MeshList;
					pStaticObject->FindAllTreeNodes(MeshList, GWRNDNODE_MESH);

					vector<IGwRndNode*>::iterator itr = MeshList.begin();

					for( ; itr != MeshList.end(); ++itr )
					{
						IGwBaseMesh * pBaseMesh = dynamic_cast<IGwBaseMesh*>(*itr);

						if( pBaseMesh != NULL )
						{
							IGwDXMesh * pMesh = pBaseMesh->GetOrigMesh();

							GwVector * pVertices = new GwVector[pBaseMesh->GetNumVertex()];

							WORD * pIndices = new WORD[pBaseMesh->GetNumFaces() * 3];

							sBuffer * pBuffer = new sBuffer;
							pBuffer->pVertices = pVertices;
							pBuffer->nNumVertices = pBaseMesh->GetNumVertex();
							pBuffer->pIndices = pIndices;
							pBuffer->nNumIndices = pBaseMesh->GetNumFaces() * 3;
							BufferList.push_back(pBuffer);

							GwVertex_Normal * pV;
							WORD * pI;

							pMesh->LockVertexBuffer((BYTE*&)pV,D3DLOCK_READONLY);
							pMesh->LockIndexBuffer((BYTE*&)pI,D3DLOCK_READONLY);

							for( int j = 0; j < pBaseMesh->GetNumVertex(); ++j )
							{
								GwVector vNormal = pV[j].vNormal;
								D3DXVec3Normalize(&vNormal, &vNormal);
								GwVector vPos = pV[j].vPos + vNormal;
								GwVector vRes;
								D3DXVec3TransformCoord(&vRes, &vPos, pBaseMesh->GetWorldMatrix());

								pVertices[j] = vRes;
							}

							for( int j = 0; j < pBaseMesh->GetNumFaces() * 3; ++j )
							{
								pIndices[j] = pI[j];
							}

							pMesh->UnlockIndexBuffer();
							pMesh->UnlockVertexBuffer();
						}
					}
				}
			}

			if( BufferList.empty() == false )
			{
				vector<sBuffer*>::iterator itr = BufferList.begin();

				for( ; itr != BufferList.end(); ++itr )
				{
					sBuffer * pBuffer = (*itr);

					if( pBuffer->nNumIndices <= 6000 )
					{
						IGwDynPolygon * pDynPolygon = Gw::GetDynPolygonMgr()->CreateDynPolygon(pBuffer->nNumVertices, pBuffer->nNumIndices);
						m_DynPolygonList.push_back(pDynPolygon);

						for( int i = 0; i < pBuffer->nNumVertices; ++i )
						{
							pDynPolygon->SetVertexPos(i, pBuffer->pVertices[i]);
							pDynPolygon->SetVertexClr(i, GwColor(1, 0, 1, 1));
						}

						for( int i = 0; i < pBuffer->nNumIndices; ++i )
						{
							pDynPolygon->SetVertexIndexList(i, pBuffer->pIndices[i]);			// 인덱스 리스트..
						}
					}
					else 
					{
						vector<sBuffer*> BufferList = pBuffer->GetBufferList();

						for( int j = 0; j < (int)BufferList.size(); ++j )
						{
							sBuffer * pBuffer_Split = BufferList[j];

							IGwDynPolygon * pDynPolygon = Gw::GetDynPolygonMgr()->CreateDynPolygon(pBuffer_Split->nNumVertices, pBuffer_Split->nNumIndices);
							m_DynPolygonList.push_back(pDynPolygon);

							for( int i = 0; i < pBuffer_Split->nNumVertices; ++i )
							{
								pDynPolygon->SetVertexPos(i, pBuffer_Split->pVertices[i]);
								pDynPolygon->SetVertexClr(i, GwColor(1, 0, 1, 1));
							}

							for( int i = 0; i < pBuffer_Split->nNumIndices; ++i )
							{
								pDynPolygon->SetVertexIndexList(i, pBuffer_Split->pIndices[i]);			// 인덱스 리스트..
							}

							delete [] pBuffer_Split->pVertices;
							delete [] pBuffer_Split->pIndices;
						}
					}

					// 해제
					delete [] pBuffer->pVertices;
					delete [] pBuffer->pIndices;
					delete (*itr);
				}

				BufferList.clear();
			}
		}
	}
}

void	CPickingObjectWireFrameRender::Render(float fDeltaTime)
{
	if( m_pSelGameObject->IsVisible() == true && m_IsRender == true )
	{
		vector<IGwDynPolygon*>::iterator itr = m_DynPolygonList.begin();

		for( ; itr != m_DynPolygonList.end(); ++itr )
		{
			(*itr)->Render(fDeltaTime);
		}
	}
}

void	CPickingObjectWireFrameRender::SetSelGameObject(IGameObject * pGameObject)
{
	m_pSelGameObject->Release();

	if( pGameObject != NULL )
	{
		m_pSelGameObject = pGameObject;
		m_pSelGameObject->AddRef();
	}
	else 
	{
		m_pSelGameObject = IGameObjectMgr::GetInstance()->GetEmptyGameObject();
		m_pSelGameObject->AddRef();
	}

	m_IsUpdate = true;
}