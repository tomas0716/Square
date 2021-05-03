#pragma once

class CGameEvent_DokkaebiFire_CameraShaking : public IGwRefObject
{
public:
	CGameEvent_DokkaebiFire_CameraShaking(IGwCamera * pCamera, float fStartDelay, float fShakingTime, float fRadius, int nShakingStrength);
	~CGameEvent_DokkaebiFire_CameraShaking();

	virtual	void	Update(float fDeltaTime);
	virtual	void	Render3D(float fDeltaTime);
	virtual	void	PostRender3D(float fDeltaTime);
	virtual	void	Render2D(float fDeltaTime);

	void			OnDone_StartDelay();
	void			OnDone_Shaking();

private:

	IGwCamera			*	m_pCamera;

	float							m_fShakingTime;
	float							m_fRadius;
	int								m_nShakingStrength;

	IGwEventTransformer_Scalar	*	m_pTimer_StartDelay;
	IGwEventTransformer_Scalar	*	m_pTimer_Shaking;

	IGwEventTransformer_Pos		*	m_pPos_Eye;
	IGwEventTransformer_Pos		*	m_pPos_Look;

	class CCallback_StartDelay;
	class CCallback_Shaking;
};


class	CEsToolApplication : public CApplication
{
public:
	CEsToolApplication(void);
	virtual ~CEsToolApplication(void);

	virtual bool		OnCreateDevice();
	virtual bool		OnResetDevice();
	virtual bool		OnLostDevice();
	virtual bool		OnDeleteDevice();

	virtual bool		Update(float fElapsedTime);
	virtual bool		Render(float fElapsedTime);

private:
	float				m_fDelta;

	CGameEvent_DokkaebiFire_CameraShaking *	m_pShaking;
};