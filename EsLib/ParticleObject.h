#pragma once

class IParticleObject : public IGameObject
{
	I_DECLARE_DYNCREATE(IParticleObject);
public:
	IParticleObject();
	IParticleObject(const char * pName);
	virtual ~IParticleObject();

	void				Update(float fDeltaTime);
	void				Render(float fDeltaTime);

	void				OnPlay();
	void				OnPause();
	void				OnStop();

	void				SetCamera(IGwCamera * pCamera);
	IGwCamera		*	GetCamera();

	void				SetLifeTime(float fMinTime, float fMaxTime);
	void				SetLifeTime_Min(float fTime);
	void				SetLifeTime_Max(float fTime);
	IRandomF		*	GetLifeTime();

	void				SetEmissionRateTime(float fMinTime, float fMaxTime);
	void				SetEmissionRateTime_Min(float fTime);
	void				SetEmissionRateTime_Max(float fTime);
	IRandomF		*	GetEmissionRateTime();

	void				SetEmissionRate(int nMin, int nMax);
	void				SetEmissionRate_Min(int nMin);
	void				SetEmissionRate_Max(int nMax);
	IRandomI		*	GetEmissionRate();

	void				SetEmissionCount(int nMin, float nMax);
	void				SetEmissionCount_Min(int nMin);
	void				SetEmissionCount_Max(int nMax);
	IRandomI		*	GetEmissionCount();

	void				SetEmissionLifeTime(float fMinTime, float fMaxTime);
	void				SetEmissionLifeTime_Min(float fTime);
	void				SetEmissionLifeTime_Max(float fTime);
	IRandomF		*	GetEmissionLifeTime();

	void				SetLoop(bool IsLoop);
	bool				IsLoop();

	void				SetStartDelay(float fMinTime, float fMaxTime);
	void				SetStartDelay_Min(float fTime);
	void				SetStartDelay_Max(float fTime);
	IRandomF		*	GetStartDelay();

	void				SetStartSpeed(float fMinSpeed, float fMaxSpeed);
	void				SetStartSpeed_Min(float fSpeed);
	void				SetStartSpeed_Max(float fSpeed);
	IRandomF		*	GetStartSpeed();

	void				SetStartScale(GwVector vMinScale, GwVector vMaxScale);
	void				SetStartScale_Min(GwVector vScale);
	void				SetStartScale_Max(GwVector vScale);
	IRandomVec		*	GetStartScale();

	void				SetStartRotation(GwVector vMinRotation, GwVector vMaxRotation);
	void				SetStartRotation_Min(GwVector vRotation);
	void				SetStartRotation_Max(GwVector vRotation);
	IRandomVec		*	GetStartRotation();

	void				SetStartColor(GwColor clrMinColor, GwColor clrMaxColor);
	void				SetStartColor_Min(GwColor clrColor);
	void				SetStartColor_Max(GwColor clrColor);
	IRandomClr		*	GetStartColor();

	void				SetStartAlpha(float fMinAlpha, float fMaxAlpha);
	void				SetStartAlpha_Min(float fAlpha);
	void				SetStartAlpha_Max(float fAlpha);
	IRandomF		*	GetStartAlpha();

	void				SetGravity(float fMinGravity, float fMaxGravity);
	void				SetGravity_Min(float fGravity);
	void				SetGravity_Max(float fGravity);
	IRandomF		*	GetGravity();

	void				SetEmissionLocal(bool IsEmissionLocal);
	bool				IsEmissionLocal();

	void				SetRenderState(eEsRenderState eRenderState);
	eEsRenderState		GetRenderState();

	void				SetEmissionShapeClassName(const char * pszClassName);
	const char		*	GetEmissionShapeClassName();
	IEmissionShape	*	GetEmissionShape();

	void				SetParticleInfoClassName(const char * pszClassName);
	const char		*	GetParticleInfoClassName();
	IParticleObjectInfo	*	GetParticleObjectInfo();

	IParticleSystem	*	GetParticleSystem();

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

protected:

	void				Initialize();
	
	void				SetValid(bool IsValid);

	bool				m_IsValid;

	IRandomF			m_rfLifeTime;
	IRandomF			m_riEmissionRateTime;
	IRandomI			m_riEmissionRate;
	IRandomI			m_riEmissionCount;
	IRandomF			m_rfEmissionLifeTime;
	bool				m_IsLoop;
	IRandomF			m_rfStartDelay;
	IRandomF			m_rfStartSpeed;
	IRandomVec			m_rvStartScale;
	IRandomVec			m_rvStartRotation;
	IRandomClr			m_rclrStartColor;
	IRandomF			m_rfStartAlpha;
	IRandomF			m_rfGravity;
	bool				m_IsEmissionLocal;

	eEsRenderState		m_eRenderState;

	IEmissionShape	*	m_pEmissionShape;
	IParticleObjectInfo	*	m_pParticleObjectInfo;
	
	IParticleSystem	*	m_pParticleSystem;

	IGwCamera		*	m_pCamera;
};