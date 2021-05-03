#include "stdafx.h"
#include "ParticleObject.h"

I_IMPLEMENT_DYNCREATE(IParticleObject, ISingletonObject::GetTypeModule())

IParticleObject::IParticleObject()
: m_IsValid(true), m_IsLoop(true), m_IsEmissionLocal(false), m_eRenderState(eEsRenderState_AlphaBlending)
{
	Initialize();
}

IParticleObject::IParticleObject(const char * pName)
: IGameObject(pName), m_IsValid(true), m_IsLoop(true), m_IsEmissionLocal(false), m_eRenderState(eEsRenderState_AlphaBlending)
{
	Initialize();
}

IParticleObject::~IParticleObject()
{
	m_pCamera->Release();
	m_pEmissionShape->Release();
	m_pParticleObjectInfo->Release();
	m_pParticleSystem->Release();
}

void				IParticleObject::Update(float fDeltaTime)
{
	m_pParticleSystem->Update(fDeltaTime);

	if( m_IsValid == false )
	{
		m_IsValid = true;
		m_pParticleSystem->ResetProperty();
		m_pParticleObjectInfo->SetRenderState(m_eRenderState);
		m_pParticleObjectInfo->CreateObject();
	}
}

void				IParticleObject::Render(float fDeltaTime)
{
	if( IsVisible() == true )
	{
		m_pParticleSystem->Render(fDeltaTime);

		//Gw::GetBillboardMgr()->SetCamera(m_pCamera);
	}
}

void				IParticleObject::Initialize()
{
	// 객체 초기화
	m_pCamera = Gw::GetCameraMgr()->GetCamera_Null()->Clone(GWCLONE_DEFAULT);
	m_pEmissionShape = new IEmissionShape();
	m_pParticleObjectInfo = new IParticleObjectInfo_Billboard();
	//m_pParticleObjectInfo = new IParticleObjectInfo_Mesh();
	m_pParticleObjectInfo->CreateObject();
	m_pParticleSystem = new IParticleSystem(this);

	// 변수 초기화
	m_rfLifeTime.SetRange(30,30);
	m_riEmissionRateTime.SetRange(1,1);
	m_riEmissionRate.SetRange(1,1);
	m_riEmissionCount.SetRange(1,1);
	m_rfEmissionLifeTime.SetRange(30,30);
	m_rfStartDelay.SetRange(0,0);
	m_rfStartSpeed.SetRange(4000,4000);
	//m_rfStartSpeed.SetRange(10,20);
	m_rvStartScale.SetRange(GwVector(0.5f,0.5f,0.5f),GwVector(0.5f,0.5f,0.5f));
	//m_rvStartScale.SetRange(GwVector(1,1,1),GwVector(1,1,1));
	m_rvStartRotation.SetRange(GwVector(0,0,0),GwVector(0,0,0));
	m_rclrStartColor.SetRange(GwColor(0.5f,0.5f,1,1),GwColor(0.6f,0.6f,1,1));
	m_rfStartAlpha.SetRange(1,1);
	m_rfGravity.SetRange(0.098f,0.098f);
	//m_rfGravity.SetRange(0,0);

	// 콤포넌트 초기화
	IComponent_Trans * pComponent_Trans = NULL;

	pComponent_Trans = new IComponent_Trans_Scale(this);
	pComponent_Trans->SetEnable(false);
	AddComponent(pComponent_Trans);		pComponent_Trans->Release();

	pComponent_Trans = new IComponent_Trans_Color(this);
	pComponent_Trans->SetEnable(false);
	AddComponent(pComponent_Trans);		pComponent_Trans->Release();

	pComponent_Trans = new IComponent_Trans_Alpha(this);
	pComponent_Trans->SetEnable(false);
	AddComponent(pComponent_Trans);		pComponent_Trans->Release();

	pComponent_Trans = new IComponent_Trans_Rotation(this);
	pComponent_Trans->SetEnable(false);
	AddComponent(pComponent_Trans);		pComponent_Trans->Release();

	pComponent_Trans = new IComponent_Trans_AnimSequence(this);
	pComponent_Trans->SetEnable(false);
	AddComponent(pComponent_Trans);		pComponent_Trans->Release();

	// 파티클시스템 리셋
	m_pParticleSystem->ResetProperty();
}

void				IParticleObject::OnPlay()
{
	m_pParticleSystem->OnPlay();
}

void				IParticleObject::OnPause()
{
	m_pParticleSystem->OnPause();
}

void				IParticleObject::OnStop()
{
	m_pParticleSystem->OnStop();
}

void				IParticleObject::SetCamera(IGwCamera * pCamera)
{
	m_pCamera->Release();
	m_pCamera = pCamera;
	m_pCamera->AddRef();
}

IGwCamera		*	IParticleObject::GetCamera()
{
	return m_pCamera;
}

void				IParticleObject::SetValid(bool IsValid)
{
	m_IsValid = IsValid;
}

void				IParticleObject::SetLifeTime(float fMinTime, float fMaxTime)
{
	m_rfLifeTime.SetRange(fMinTime, fMaxTime);
	SetValid(false);
}

void				IParticleObject::SetLifeTime_Min(float fTime)
{
	m_rfLifeTime.SetMin(fTime);
	SetValid(false);
}

void				IParticleObject::SetLifeTime_Max(float fTime)
{
	m_rfLifeTime.SetMax(fTime);
	SetValid(false);
}

IRandomF		*	IParticleObject::GetLifeTime()
{
	return &m_rfLifeTime;
}	

void				IParticleObject::SetEmissionRateTime(float fMinTime, float fMaxTime)
{
	m_riEmissionRateTime.SetRange(fMinTime, fMaxTime);
	SetValid(false);
}

void				IParticleObject::SetEmissionRateTime_Min(float fTime)
{
	m_riEmissionRateTime.SetMin(fTime);
	SetValid(false);
}

void				IParticleObject::SetEmissionRateTime_Max(float fTime)
{
	m_riEmissionRateTime.SetMax(fTime);
	SetValid(false);
}

IRandomF		*	IParticleObject::GetEmissionRateTime()
{
	return &m_riEmissionRateTime;
}

void				IParticleObject::SetEmissionRate(int nMin, int nMax)
{
	m_riEmissionRate.SetRange(nMin, nMax);
	SetValid(false);
}

void				IParticleObject::SetEmissionRate_Min(int nMin)
{
	m_riEmissionRate.SetMin(nMin);
	SetValid(false);
}

void				IParticleObject::SetEmissionRate_Max(int nMax)
{
	m_riEmissionRate.SetMax(nMax);
	SetValid(false);
}

IRandomI		*	IParticleObject::GetEmissionRate()
{
	return &m_riEmissionRate;
}

void				IParticleObject::SetEmissionCount(int nMin, float nMax)
{
	m_riEmissionCount.SetRange(nMin, nMax);
	SetValid(false);
}

void				IParticleObject::SetEmissionCount_Min(int nMin)
{
	m_riEmissionCount.SetMin(nMin);
	SetValid(false);
}

void				IParticleObject::SetEmissionCount_Max(int nMax)
{
	m_riEmissionCount.SetMax(nMax);
	SetValid(false);
}

IRandomI		*	IParticleObject::GetEmissionCount()
{
	return &m_riEmissionCount;
}

void				IParticleObject::SetEmissionLifeTime(float fMinTime, float fMaxTime)
{
	m_rfEmissionLifeTime.SetRange(fMinTime, fMaxTime);
	SetValid(false);
}

void				IParticleObject::SetEmissionLifeTime_Min(float fTime)
{
	m_rfEmissionLifeTime.SetMin(fTime);
	SetValid(false);
}

void				IParticleObject::SetEmissionLifeTime_Max(float fTime)
{
	m_rfEmissionLifeTime.SetMax(fTime);
	SetValid(false);
}

IRandomF		*	IParticleObject::GetEmissionLifeTime()
{
	return &m_rfEmissionLifeTime;
}	


void				IParticleObject::SetLoop(bool IsLoop)
{
	m_IsLoop = IsLoop;
	SetValid(false);
}

bool				IParticleObject::IsLoop()
{
	return m_IsLoop;
}

void				IParticleObject::SetStartDelay(float fMinTime, float fMaxTime)
{
	m_rfStartDelay.SetRange(fMinTime, fMaxTime);
	SetValid(false);
}

void				IParticleObject::SetStartDelay_Min(float fTime)
{
	m_rfStartDelay.SetMin(fTime);
	SetValid(false);
}

void				IParticleObject::SetStartDelay_Max(float fTime)
{
	m_rfStartDelay.SetMax(fTime);
	SetValid(false);
}

IRandomF		*	IParticleObject::GetStartDelay()
{
	return &m_rfStartDelay;
}

void				IParticleObject::SetStartSpeed(float fMinSpeed, float fMaxSpeed)
{
	m_rfStartSpeed.SetRange(fMinSpeed, fMaxSpeed);
	SetValid(false);
}

void				IParticleObject::SetStartSpeed_Min(float fSpeed)
{
	m_rfStartSpeed.SetMin(fSpeed);
	SetValid(false);
}

void				IParticleObject::SetStartSpeed_Max(float fSpeed)
{
	m_rfStartSpeed.SetMax(fSpeed);
	SetValid(false);
}

IRandomF		*	IParticleObject::GetStartSpeed()
{
	return &m_rfStartSpeed;
}

void				IParticleObject::SetStartScale(GwVector vMinScale, GwVector vMaxScale)
{
	m_rvStartScale.SetRange(vMinScale, vMaxScale);
	SetValid(false);
}

void				IParticleObject::SetStartScale_Min(GwVector vScale)
{
	m_rvStartScale.SetMin(vScale);
	SetValid(false);
}

void				IParticleObject::SetStartScale_Max(GwVector vScale)
{
	m_rvStartScale.SetMax(vScale);
	SetValid(false);
}

IRandomVec		*	IParticleObject::GetStartScale()
{
	return &m_rvStartScale;
}

void				IParticleObject::SetStartRotation(GwVector vMinRotation, GwVector vMaxRotation)
{
	m_rvStartRotation.SetRange(vMinRotation, vMaxRotation);
	SetValid(false);
}

void				IParticleObject::SetStartRotation_Min(GwVector vRotation)
{
	m_rvStartRotation.SetMin(vRotation);
	SetValid(false);
}

void				IParticleObject::SetStartRotation_Max(GwVector vRotation)
{
	m_rvStartRotation.SetMax(vRotation);
	SetValid(false);
}

IRandomVec		*	IParticleObject::GetStartRotation()
{
	return &m_rvStartRotation;
}

void				IParticleObject::SetStartColor(GwColor clrMinColor, GwColor clrMaxColor)
{
	m_rclrStartColor.SetRange(clrMinColor, clrMaxColor);
	SetValid(false);
}

void				IParticleObject::SetStartColor_Min(GwColor clrColor)
{
	m_rclrStartColor.SetMin(clrColor);
	SetValid(false);
}

void				IParticleObject::SetStartColor_Max(GwColor clrColor)
{
	m_rclrStartColor.SetMax(clrColor);
	SetValid(false);
}

IRandomClr		*	IParticleObject::GetStartColor()
{
	return &m_rclrStartColor;
}

void				IParticleObject::SetStartAlpha(float fMinAlpha, float fMaxAlpha)
{
	m_rfStartAlpha.SetRange(fMinAlpha, fMaxAlpha);
	SetValid(false);
}

void				IParticleObject::SetStartAlpha_Min(float fAlpha)
{
	m_rfStartAlpha.SetMin(fAlpha);
	SetValid(false);
}

void				IParticleObject::SetStartAlpha_Max(float fAlpha)
{
	m_rfStartAlpha.SetMax(fAlpha);
	SetValid(false);
}

IRandomF		*	IParticleObject::GetStartAlpha()
{
	return &m_rfStartAlpha;
}

void				IParticleObject::SetGravity(float fMinGravity, float fMaxGravity)
{
	m_rfGravity.SetRange(fMinGravity, fMaxGravity);
	SetValid(false);
}

void				IParticleObject::SetGravity_Min(float fGravity)
{
	m_rfGravity.SetMin(fGravity);
	SetValid(false);
}

void				IParticleObject::SetGravity_Max(float fGravity)
{
	m_rfGravity.SetMax(fGravity);
	SetValid(false);
}

IRandomF		*	IParticleObject::GetGravity()
{
	return &m_rfGravity;
}

void				IParticleObject::SetEmissionLocal(bool IsEmissionLocal)
{
	m_IsEmissionLocal = IsEmissionLocal;
	SetValid(false);
}

bool				IParticleObject::IsEmissionLocal()
{
	return m_IsEmissionLocal;
}

void				IParticleObject::SetRenderState(eEsRenderState eRenderState)
{
	m_eRenderState = eRenderState;
	SetValid(false);
}

eEsRenderState		IParticleObject::GetRenderState()
{
	return m_eRenderState;
}

void				IParticleObject::SetEmissionShapeClassName(const char * pszClassName)
{
	m_pEmissionShape->Release();
	IType * pType = ITypeManager::GetInstance()->FindType(pszClassName);
	m_pEmissionShape = dynamic_cast<IEmissionShape*>(pType->CreateInstance());
}

const char		*	IParticleObject::GetEmissionShapeClassName()
{
	const char * pszClassName = IHashObject::GetInstance()->GetHashString(m_pEmissionShape->GetClassHashCode());

	return pszClassName;
}

IEmissionShape	*	IParticleObject::GetEmissionShape()
{
	return m_pEmissionShape;
}

void				IParticleObject::SetParticleInfoClassName(const char * pszClassName)
{
	m_pParticleObjectInfo->Release();
	IType * pType = ITypeManager::GetInstance()->FindType(pszClassName);
	m_pParticleObjectInfo = dynamic_cast<IParticleObjectInfo*>(pType->CreateInstance());
	m_pParticleObjectInfo->CreateObject();
}

const char		*	IParticleObject::GetParticleInfoClassName()
{
	const char * pszClassName = IHashObject::GetInstance()->GetHashString(m_pParticleObjectInfo->GetClassHashCode());

	return pszClassName;
}

IParticleObjectInfo	*	IParticleObject::GetParticleObjectInfo()
{
	return m_pParticleObjectInfo;
}

IParticleSystem	*	IParticleObject::GetParticleSystem()
{
	return m_pParticleSystem;
}

BYTE	*			IParticleObject::LoadFile(BYTE * pData)
{
	pData = IGameObject::LoadFile(pData);

	char szClassName[64] = { 0, };
	IType * pType = NULL;

	memcpy(&m_IsLoop, pData, sizeof(bool)); 	pData += sizeof(bool);

	pData = m_rfLifeTime.LoadFile(pData);
	pData = m_riEmissionRateTime.LoadFile(pData);
	pData = m_riEmissionRate.LoadFile(pData);
	pData = m_riEmissionCount.LoadFile(pData);
	pData = m_rfEmissionLifeTime.LoadFile(pData);
	pData = m_rfStartDelay.LoadFile(pData);
	pData = m_rfStartSpeed.LoadFile(pData);
	pData = m_rvStartScale.LoadFile(pData);
	pData = m_rvStartRotation.LoadFile(pData);
	pData = m_rclrStartColor.LoadFile(pData);
	pData = m_rfStartAlpha.LoadFile(pData);
	pData = m_rfGravity.LoadFile(pData);

	memcpy(&m_IsEmissionLocal, pData, sizeof(bool)); pData += sizeof(bool);
	memcpy(&m_eRenderState, pData, sizeof(eEsRenderState)); pData += sizeof(eEsRenderState);

	m_pEmissionShape->Release();
	memcpy(szClassName, pData, sizeof(char) * 64); pData += sizeof(char) * 64;
	pType = ITypeManager::GetInstance()->FindType(szClassName);
	m_pEmissionShape = dynamic_cast<IEmissionShape*>(pType->CreateInstance());
	pData = m_pEmissionShape->LoadFile(pData);

	m_pParticleObjectInfo->Release();
	memcpy(szClassName, pData, sizeof(char) * 64); pData += sizeof(char) * 64;
	pType = ITypeManager::GetInstance()->FindType(szClassName);
	m_pParticleObjectInfo = dynamic_cast<IParticleObjectInfo*>(pType->CreateInstance());
	pData = m_pParticleObjectInfo->LoadFile(pData);
	m_pParticleObjectInfo->CreateObject();

	SetValid(false);

	return pData;
}

void				IParticleObject::SaveFile(IFile & kFile)
{
	IGameObject::SaveFile(kFile);

	kFile.WriteFile(&m_IsLoop, sizeof(bool));

	m_rfLifeTime.SaveFile(kFile);
	m_riEmissionRateTime.SaveFile(kFile);
	m_riEmissionRate.SaveFile(kFile);
	m_riEmissionCount.SaveFile(kFile);
	m_rfEmissionLifeTime.SaveFile(kFile);
	m_rfStartDelay.SaveFile(kFile);
	m_rfStartSpeed.SaveFile(kFile);
	m_rvStartScale.SaveFile(kFile);
	m_rvStartRotation.SaveFile(kFile);
	m_rclrStartColor.SaveFile(kFile);
	m_rfStartAlpha.SaveFile(kFile);
	m_rfGravity.SaveFile(kFile);

	kFile.WriteFile(&m_IsEmissionLocal, sizeof(bool));
	kFile.WriteFile(&m_eRenderState, sizeof(eEsRenderState));

	const char * pszClassName = IHashObject::GetInstance()->GetHashString(m_pEmissionShape->GetClassHashCode());
	kFile.WriteFile((void*)pszClassName, sizeof(char) * 64);
	m_pEmissionShape->SaveFile(kFile);

	pszClassName = IHashObject::GetInstance()->GetHashString(m_pParticleObjectInfo->GetClassHashCode());
	kFile.WriteFile((void*)pszClassName, sizeof(char) * 64);

	m_pParticleObjectInfo->SaveFile(kFile);
}