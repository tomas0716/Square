#include "StdAfx.h"
#include "PathObject.h"

I_IMPLEMENT_DYNCREATE(IPathObject, ISingletonObject::GetTypeModule())

#define FRAME_RATE 0.01666666f

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IEventTransformerCallback_Path

class IPathObject::IEventTransformerCallback_Path : public ITransformer_Callback
{
public:
	IEventTransformerCallback_Path(IPathObject* pPathObject) : m_nIndex(0)
	{
		m_pPathObject = pPathObject;
	}

	virtual void	OnOneEventDone(int nEventIndex) 
	{
		m_pPathObject->OnOneEventDone(m_nIndex++);
	}

	virtual	void	OnCompleteEventDone()
	{
		m_pPathObject->OnEventDone();
	}

private:
	IPathObject	*	m_pPathObject;
	int				m_nIndex;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// IPathObject

IPathObject::IPathObject()
: IGameObject(), m_bBlockTime(true), m_fEntireTime(3.f), m_ePlayType(ePlay_Stop), m_fFrameRate(1),
   m_IsCreationAutoPlay(false), m_IsAttachGameObject(false), m_pAttachGameObject(NULL), 
   m_vTransformerPos(GwVector(0,0,0))
{
	m_pTransformer = new ITransformer_Pos(GwVector(0,0,0));

	m_pDelegate_OneDot = new IDelegate();
	m_pDelegate_Complete = new IDelegate();
}

IPathObject::IPathObject(const char * pName)
: IGameObject(pName), m_bBlockTime(true), m_fEntireTime(3.f), m_ePlayType(ePlay_Stop), m_fFrameRate(1),
  m_IsCreationAutoPlay(false), m_IsAttachGameObject(false), m_pAttachGameObject(NULL), 
  m_vTransformerPos(GwVector(0,0,0))
{
	m_pTransformer = new ITransformer_Pos(GwVector(0,0,0));

	m_pDelegate_OneDot = new IDelegate();
	m_pDelegate_Complete = new IDelegate();
}

IPathObject::~IPathObject(void)
{
	vector<sPathDotInfo*>::iterator itr = m_PathDotInfoList.begin();

	for( int i = 0; itr != m_PathDotInfoList.end(); ++itr, ++i )
	{
		sPathDotInfo * pPathDotInfo = (*itr);
		delete pPathDotInfo;
	}

	m_PathDotInfoList.clear();

	m_pTransformer->Release();
	m_pDelegate_OneDot->Release();
	m_pDelegate_Complete->Release();

	if( m_pAttachGameObject != NULL ) m_pAttachGameObject->Release();
}

void						IPathObject::Update(float fDeltaTime)
{
	if( m_ePlayType == ePlay_Play )
	{
		m_pTransformer->Update(fDeltaTime);

		if( m_IsAttachGameObject == true )
		{
			GwVector vPos = m_pTransformer->GetCurrPos();
			m_pAttachGameObject->SetPosition(vPos);

			if( vPos != m_vTransformerPos )
			{
				GwVector vDir, vDot;
				vDir = vDot = vPos - m_vTransformerPos;
				vDot.y = 0;
				m_vTransformerPos = vPos;
				
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vDot, &vDot);

				float fDot = D3DXVec3Dot(&vDot, &GwVector(0,0,-1));
				if( _finite(fDot) == false )	
					return;

				float fAngle = acosf(fDot);
				if( _finite(fAngle) == false )
					return;

				if( vDir.x > 0 ) 
					fAngle *= -1;

				m_pAttachGameObject->SetRotation(GwVector(D3DXToDegree(vDir.y), D3DXToDegree(fAngle), 0));
			}
		}
	}
	else if( m_ePlayType == ePlay_Stop )
	{
		if( m_pTransformer->GetNumEvent() >= 2 && m_IsAttachGameObject == true )
		{
			ITransformerEvent_Pos * pEvent_01 = dynamic_cast<ITransformerEvent_Pos*>(m_pTransformer->GetEvent(0));
			ITransformerEvent_Pos * pEvent_02 = dynamic_cast<ITransformerEvent_Pos*>(m_pTransformer->GetEvent(1));

			if( pEvent_01 != NULL && pEvent_02 != NULL )
			{
				GwVector vPos_01 = pEvent_01->GetPos();
				GwVector vPos_02 = pEvent_02->GetPos();

				m_pAttachGameObject->SetPosition(vPos_01);

				GwVector vDir, vDot;
				vDir = vDot = vPos_02 - vPos_01;
				vDot.y = 0;
				
				D3DXVec3Normalize(&vDir, &vDir);
				D3DXVec3Normalize(&vDot, &vDot);

				float fDot = D3DXVec3Dot(&vDot, &GwVector(0,0,-1));
				float fAngle = acosf(fDot);

				if( fAngle > FLT_MAX || fAngle < -FLT_MAX )
				{
					return;
				}

				if( vDir.x > 0 ) 
					fAngle *= -1;

				m_pAttachGameObject->SetRotation(GwVector(D3DXToDegree(vDir.y), D3DXToDegree(fAngle), 0));
			}
		}
	}

	if( m_IsAttachGameObject == true && m_pAttachGameObject != NULL )
		m_pAttachGameObject->Update(fDeltaTime);
}

void						IPathObject::Render(float fDeltaTime)
{
	if( IsVisible() == true && m_IsAttachGameObject == true )
	{
		m_pAttachGameObject->Render(fDeltaTime);
	}
}

void						IPathObject::Render(float fDeltaTime, IGwViewFrustumOct * pFrustum)
{
	if( IsVisible() == true && m_IsAttachGameObject == true )
	{
		// 클라이언트에서 컬링 해야 함.
		m_pAttachGameObject->Render(fDeltaTime);

		IGameObject::Render(fDeltaTime, pFrustum);
	}
}


void						IPathObject::OnPlay()
{
	if( m_ePlayType == ePlay_Stop || m_ePlayType == ePlay_Complete )
	{
		IEventTransformerCallback_Path * pCallback = new IEventTransformerCallback_Path(this);
		m_pTransformer->SetCallback(pCallback); pCallback->Release();

		m_pTransformer->OnStop();
		m_pTransformer->OnPlay();

		if( m_pTransformer->GetNumEvent() >= 1 )
		{
			ITransformerEvent_Pos * pEvent = dynamic_cast<ITransformerEvent_Pos*>(m_pTransformer->GetEvent(0));
			m_vTransformerPos = pEvent->GetPos();
		}
	}
	
	m_ePlayType = ePlay_Play;
}

void						IPathObject::OnStop()
{
	if( m_ePlayType == ePlay_Play || m_ePlayType == ePlay_Pause )
	{
		m_pTransformer->OnStop();
	}

	m_ePlayType = ePlay_Stop;
}

void						IPathObject::OnPause()
{
	if( m_ePlayType == ePlay_Play )
		m_ePlayType = ePlay_Pause;
}

ePlayType					IPathObject::GetCurrPlayType()
{
	return m_ePlayType;
}

void						IPathObject::AddEvent(sPathDotInfo * pPathDotInfo)
{
	m_PathDotInfoList.push_back(pPathDotInfo);

	ResetEventIndex();
}

void						IPathObject::AddEvent(float fTime, GwVector vPosition, bool bController)
{
	sPathDotInfo * pPathDotInfo = new sPathDotInfo;
	pPathDotInfo->fTime = fTime;
	pPathDotInfo->vLocalPos = vPosition;
	pPathDotInfo->bController = bController;
	D3DXVec3TransformCoord(&pPathDotInfo->vWorldPos, &vPosition, &GetWorldMat());
	
	AddEvent(pPathDotInfo);
}

void						IPathObject::InsertEvent(int nIndex, sPathDotInfo * pPathDotInfo)
{
	if( nIndex >= (int)m_PathDotInfoList.size() )
	{
		AddEvent(pPathDotInfo);
	}
	else
	{
		vector<sPathDotInfo*>::iterator itr = m_PathDotInfoList.begin();

		for( int i = 0; i < nIndex; ++i )
		{
			++itr;
		}

		m_PathDotInfoList.insert(itr, pPathDotInfo);
	}

	ResetEventIndex();
}

void						IPathObject::InsertEvent(int nIndex, float fTime, GwVector vPosition, bool bController)
{
	sPathDotInfo * pPathDotInfo = new sPathDotInfo;
	pPathDotInfo->fTime = fTime;
	pPathDotInfo->vLocalPos = vPosition;
	pPathDotInfo->bController = bController;
	D3DXVec3TransformCoord(&pPathDotInfo->vWorldPos, &vPosition, &GetWorldMat());

	InsertEvent(nIndex, pPathDotInfo);
}

void						IPathObject::EventTimeAdd(int nIndex)
{
	vector<sPathDotInfo*>::iterator itr = m_PathDotInfoList.begin();

	for( int i = 0; itr != m_PathDotInfoList.end(); ++itr, ++i )
	{
		if( i >= nIndex )
		{
			sPathDotInfo * pPathDotInfo = (*itr);

			if( pPathDotInfo->bController == false )
				pPathDotInfo->fTime += 1.f;
		}
	}

}

void						IPathObject::DeleteEvent(int nIndex)
{
	vector<sPathDotInfo*>::iterator itr = m_PathDotInfoList.begin();

	for( int i = 0; itr != m_PathDotInfoList.end(); ++itr, ++i )
	{
		if( i == nIndex )
		{
			sPathDotInfo * pPathDotInfo = (*itr);

			delete pPathDotInfo;

			m_PathDotInfoList.erase(itr);

			break;
		}
	}

	ResetEventIndex();
}

void						IPathObject::DeleteEvent(sPathDotInfo * pPathDotInfo)
{
	vector<sPathDotInfo*>::iterator itr = m_PathDotInfoList.begin();

	for( ; itr != m_PathDotInfoList.end(); ++itr )
	{
		sPathDotInfo * pSrcEvent = (*itr);
		if( pSrcEvent == pPathDotInfo )
		{
			delete pSrcEvent;

			m_PathDotInfoList.erase(itr);

			break;
		}
	}

	ResetEventIndex();
}

void						IPathObject::DeleteEventAll()
{
	vector<sPathDotInfo*>::iterator itr = m_PathDotInfoList.begin();

	for( ; itr != m_PathDotInfoList.end(); ++itr )
	{
		sPathDotInfo * pSrcEvent = (*itr);
		if( pSrcEvent )
		{
			delete pSrcEvent;
		}
	}

	m_PathDotInfoList.clear();
}

int							IPathObject::GetNumEvent()
{
	return (int)m_PathDotInfoList.size();
}

sPathDotInfo			*	IPathObject::GetEvent(int nIndex)
{
	if( nIndex >= (int)m_PathDotInfoList.size() || nIndex < 0 )
		return NULL;

	return m_PathDotInfoList[nIndex];
}

void						IPathObject::ResetEventIndex()
{
	int nNumData = (int)m_PathDotInfoList.size();

	for( int i = 0; i < nNumData; ++i )
	{
		sPathDotInfo * pPathDotInfo = m_PathDotInfoList[i];

		if( pPathDotInfo )
		{
			pPathDotInfo->nIndex = i;
		}
	}
}

void						IPathObject::OnOneEventDone(int nIndex)
{
	for( int i = 0; i < (int)m_BlockEndIndexList.size(); ++i )
	{
		if( nIndex == m_BlockEndIndexList[i] )
		{
			(*m_pDelegate_OneDot)(i+1);

			return;
		}
	}
}

void						IPathObject::OnEventDone()
{
	m_ePlayType = ePlay_Complete;

	(*m_pDelegate_Complete)();
}

void						IPathObject::ReLoad()
{
	int nNumData = (int)m_PathDotInfoList.size();

	for( int i = 0; i < nNumData; ++i )
	{
		sPathDotInfo * pPathDotInfo = m_PathDotInfoList[i];
		D3DXVec3TransformCoord(&pPathDotInfo->vWorldPos, &pPathDotInfo->vLocalPos, &GetWorldMat());
	}

	if( m_bBlockTime )
	{
		ReLoad_BlockTime();
	}
	else
	{
		ReLoad_NoneBlockTime();
	}	
}

void						IPathObject::SetBlockTime(bool bBlockTime)
{
	m_bBlockTime = bBlockTime;
}

bool						IPathObject::GetBlockTime()
{
	return m_bBlockTime;
}

void						IPathObject::SetEntireTime(float fTime)
{
	m_fEntireTime= fTime;
}

float						IPathObject::GetEntireTime()
{
	return m_fEntireTime;
}

void 						IPathObject::SetFrameRate(float FrameRate)
{
	m_fFrameRate = FrameRate;
}

float 						IPathObject::GetFrameRate()
{
	return m_fFrameRate;
}	

void						IPathObject::ReLoad_NoneBlockTime()
{
	if( m_PathDotInfoList.empty() == true )
		return;

	float fBlockTime = (m_fEntireTime / ((int)((int)m_PathDotInfoList.size() / 3)));

	IBezier4 * pBezier4 = new IBezier4();
	int * pBlockSplitIndex = new int[m_PathDotInfoList.size()];

	int nNumPos = (int)m_PathDotInfoList.size();

	for( int i = 0, block = 0; i < nNumPos; ++i )
	{
		sPathDotInfo * pEventInfo = m_PathDotInfoList[i];
		pBezier4->AddPos(pEventInfo->vWorldPos);

		if( pEventInfo->bController == false )
		{
			pBezier4->SetBlockTime(block++, fBlockTime);
		}
	}

	pBezier4->OnPlay();

	int nCount = (int)(m_fEntireTime / FRAME_RATE);

	GwVector vPos01 = m_PathDotInfoList[0]->vWorldPos;
	float fTotalLength = 0;

	for( int i = 0; i < nCount; ++i )
	{
		pBezier4->Update(FRAME_RATE);

		GwVector vPos02 = pBezier4->GetCurrPos();

		fTotalLength += D3DXVec3Length(&(vPos01 - vPos02));

		vPos01 = vPos02;
	}

	pBezier4->OnStop();
	pBezier4->OnPlay();

	nCount = (int)(m_fEntireTime / FRAME_RATE);

	vPos01 = m_PathDotInfoList[0]->vWorldPos;

	m_pTransformer->Release();
	m_pTransformer = new ITransformer_Pos(vPos01);
	
	float fTimeDelta = 0;
	float fBlockLength = 0;
	float fCurrLength = 0;

	m_BlockEndIndexList.clear();

	for( int i = 0, nBlock = 1; i < nCount; ++i )
	{
		bool IsBlockComplete = pBezier4->Update(FRAME_RATE);

		GwVector vPos02 = pBezier4->GetCurrPos();

		float fLength = D3DXVec3Length(&(vPos02 - vPos01));

		fBlockLength += fLength;
		fCurrLength += fLength;

		if( IsBlockComplete == true )
		{
			m_PathDotInfoList[nBlock * 3]->fLength = fBlockLength;
			m_PathDotInfoList[nBlock * 3]->fRealTime = (fCurrLength / fTotalLength) * m_fEntireTime;
			m_BlockEndIndexList.push_back(i);
			fBlockLength = 0;
			++nBlock;
		}

		float MoveTime = ((m_fEntireTime*m_fFrameRate) * fLength) / fTotalLength;

		ITransformerEvent_Pos*	pEventPos;
		pEventPos	=	new ITransformerEvent_Pos(MoveTime + fTimeDelta, vPos02);
		m_pTransformer->AddEvent(pEventPos);	pEventPos->Release();

		fTimeDelta += MoveTime;
		vPos01 = vPos02;
	}

	IEventTransformerCallback_Path * pCallback = new IEventTransformerCallback_Path(this);
	m_pTransformer->SetCallback(pCallback); pCallback->Release();

	pBezier4->Release();
}

int							IPathObject::LoadBlockTime(float fPrevTime, vector<sPathDotInfo*> & EventInfoList)
{
	IBezier4 * pBezier4 = new IBezier4();

	float fTempTimeDelta = EventInfoList[3]->fTime - fPrevTime;

	for( int i = 0; i < (int)EventInfoList.size(); ++i )
	{
		pBezier4->AddPos(EventInfoList[i]->vWorldPos);
	}

	pBezier4->SetBlockTime(0,fTempTimeDelta);

	pBezier4->OnPlay();

	int nCount = (int)(fTempTimeDelta / FRAME_RATE);

	GwVector vPos01 = EventInfoList[0]->vWorldPos;
	float fTotalLength = 0;

	for( int i = 0; i < nCount; ++i )
	{
		pBezier4->Update(FRAME_RATE);

		GwVector vPos02 = pBezier4->GetCurrPos();

		fTotalLength += D3DXVec3Length(&(vPos01 - vPos02));

		vPos01 = vPos02;
	}

	pBezier4->OnStop();
	pBezier4->OnPlay();

	vPos01 = EventInfoList[0]->vWorldPos;

	float fTimeDelta = 0;
	float fBlockLength = 0;

	for( int i = 0, nBlock = 1; i < nCount; ++i )
	{
		bool IsBlockComplete = pBezier4->Update(FRAME_RATE);

		GwVector vPos02 = pBezier4->GetCurrPos();

		float fLength = D3DXVec3Length(&(vPos02 - vPos01));
		fBlockLength += fLength;

		if( IsBlockComplete == true )
		{
			EventInfoList[nBlock * 3]->fLength = fBlockLength;
			fBlockLength = 0;
			++nBlock;
		}

		float MoveTime = ((fTempTimeDelta*m_fFrameRate) * fLength) / fTotalLength;

		ITransformerEvent_Pos*	pEventPos;
		pEventPos	=	new ITransformerEvent_Pos(fPrevTime + MoveTime + fTimeDelta, vPos02);
		m_pTransformer->AddEvent(pEventPos);	pEventPos->Release();

		fTimeDelta += MoveTime;
		vPos01 = vPos02;
	}

	pBezier4->Release();

	return nCount;
}

void						IPathObject::ReLoad_BlockTime()
{
	if( m_PathDotInfoList.empty() == true )
		return;

	m_pTransformer->Release();
	m_pTransformer = new ITransformer_Pos(m_PathDotInfoList[0]->vWorldPos);

	vector<sPathDotInfo*>	TempEventInfoList;

	TempEventInfoList.push_back(m_PathDotInfoList[0]);

	float	fTimeDelta = m_PathDotInfoList[0]->fTime;

	int nStartIndex = 0;
	m_BlockEndIndexList.clear();

	for( int i = 1; i < (int)(m_PathDotInfoList.size()); ++i )
	{
		if( i % 3 == 0 )
		{
			m_PathDotInfoList[i]->fRealTime = m_PathDotInfoList[i]->fTime;

			TempEventInfoList.push_back(m_PathDotInfoList[i]);

			nStartIndex += LoadBlockTime(m_PathDotInfoList[i-3]->fTime, TempEventInfoList);
			m_BlockEndIndexList.push_back(nStartIndex);

			TempEventInfoList.clear();
			TempEventInfoList.push_back(m_PathDotInfoList[i]);
		}
		else
		{
			TempEventInfoList.push_back(m_PathDotInfoList[i]);
		}
	}

	IEventTransformerCallback_Path * pCallback = new IEventTransformerCallback_Path(this);
	m_pTransformer->SetCallback(pCallback); pCallback->Release();
}

void						IPathObject::SetCallback(IDelegate * pDelegate_OneDot, IDelegate * pDelegate_Complete)
{
	if( pDelegate_OneDot != NULL )
	{
		m_pDelegate_OneDot->Release();
		m_pDelegate_OneDot = pDelegate_OneDot;
		m_pDelegate_OneDot->AddRef();
	}

	if( pDelegate_Complete != NULL )
	{
		m_pDelegate_Complete->Release();
		m_pDelegate_Complete = pDelegate_Complete;
		m_pDelegate_Complete->AddRef();
	}
}

void						IPathObject::ClearCallback()
{
	m_pDelegate_OneDot->Release();
	m_pDelegate_OneDot = new IDelegate();

	m_pDelegate_Complete->Release();
	m_pDelegate_Complete = new IDelegate();
}

GwVector					IPathObject::GetCurrPos()
{
	return m_pTransformer->GetCurrPos();
}

void						IPathObject::SetCreationAutoPlay(bool IsAutoPlay)
{
	m_IsCreationAutoPlay = IsAutoPlay;
}

bool						IPathObject::IsCreationAutoPlay()
{
	return m_IsCreationAutoPlay;
}

void						IPathObject::SetAttachGameObject(IGameObject * pGameObject)
{
	if( pGameObject != NULL )
	{
		if( m_pAttachGameObject != NULL ) m_pAttachGameObject->Release();

		m_pAttachGameObject = pGameObject;
		pGameObject->AddRef();

		m_IsAttachGameObject = true;
	}
	else
	{
		if( m_pAttachGameObject != NULL ) m_pAttachGameObject->Release();
		m_pAttachGameObject = NULL;

		m_IsAttachGameObject = false;
	}
}

bool						IPathObject::IsAttachGameObject()
{
	return m_IsAttachGameObject;
}

IGameObject				*	IPathObject::GetAttachGameObject()
{
	return m_pAttachGameObject;
}

BYTE			*			IPathObject::LoadFile(BYTE * pData)
{
	pData = IGameObject::LoadFile(pData);

	int nNumEvent;
	memcpy(&nNumEvent, pData, sizeof(int));			pData += sizeof(int);

	for( int i = 0; i < nNumEvent; ++i )
	{
		sPathDotInfo * pPathDotInfo = new sPathDotInfo;

		memcpy(&pPathDotInfo->nIndex, pData, sizeof(int));			pData += sizeof(int);
		memcpy(&pPathDotInfo->fTime, pData, sizeof(float));			pData += sizeof(float);
		memcpy(&pPathDotInfo->vLocalPos, pData, sizeof(GwVector));	pData += sizeof(GwVector);
		memcpy(&pPathDotInfo->bController, pData, sizeof(bool));	pData += sizeof(bool);

		memcpy(&m_bBlockTime, pData, sizeof(bool));					pData += sizeof(bool);
		memcpy(&m_fEntireTime, pData, sizeof(float));				pData += sizeof(float);

		m_PathDotInfoList.push_back(pPathDotInfo);
	}

	ResetEventIndex();

	ReLoad();

	memcpy(&m_IsCreationAutoPlay, pData, sizeof(bool));				pData += sizeof(bool);
	memcpy(&m_IsAttachGameObject, pData, sizeof(bool));				pData += sizeof(bool);

	if( m_IsAttachGameObject == true )
	{
		char szClassName[64] = { 0, };
		memcpy(szClassName, pData, sizeof(char) * 64);	pData += sizeof(char) * 64;

		IType * pType = ITypeManager::GetInstance()->FindType(szClassName);
		m_pAttachGameObject = dynamic_cast<IGameObject*>(pType->CreateInstance());

		pData = m_pAttachGameObject->LoadFile(pData);
	}

	if( m_IsCreationAutoPlay == true )
	{
		OnPlay();
	}

	return pData;
}

void						IPathObject::SaveFile(IFile & kFile)
{
	IGameObject::SaveFile(kFile);

	int nNumEvent = (int)m_PathDotInfoList.size();
	kFile.WriteFile(&nNumEvent, sizeof(int));

	for( int i = 0; i < nNumEvent; ++i )
	{
		sPathDotInfo * pPathDotInfo = m_PathDotInfoList[i];

		kFile.WriteFile(&pPathDotInfo->nIndex, sizeof(int));
		kFile.WriteFile(&pPathDotInfo->fTime, sizeof(float));
		kFile.WriteFile(&pPathDotInfo->vLocalPos, sizeof(GwVector));
		kFile.WriteFile(&pPathDotInfo->bController, sizeof(bool));

		kFile.WriteFile(&m_bBlockTime, sizeof(bool));
		kFile.WriteFile(&m_fEntireTime, sizeof(float));
	}

	kFile.WriteFile(&m_IsCreationAutoPlay, sizeof(bool));
	kFile.WriteFile(&m_IsAttachGameObject, sizeof(bool));
	if( m_IsAttachGameObject == true )
	{
		const char * pszClassName = IHashObject::GetInstance()->GetHashString(m_pAttachGameObject->GetClassHashCode());
		kFile.WriteFile((void*)pszClassName, sizeof(char) * 64);

		m_pAttachGameObject->SaveFile(kFile);
	}
}