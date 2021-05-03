#pragma once

class IParticleSystem;
class IParticleObject;

class IParticle : public IGwRefObject
{
private:
	IParticle(IParticleSystem * pParticleSystem, IParticleObject * pParticleObject, IGameObject * pGameObject, GwVector vParentPos, GwVector vParentRot, GwVector vParentScale, 
			  GwVector vStartPos, GwVector vStartDirection, float fEmissionLifeTime, float fStartSpeed,
			  GwVector vStartScale, GwVector vStartRotation, GwColor clrStartColor, float fStartAlpha, float fGravity, bool IsLocal);
	virtual ~IParticle();

	void				Update(float fDeltaTime);
	void				Render(float fDeltaTime);

	void				SetGameObject(IGameObject * pGameObject);
	IGameObject		*	GetGameObject();

	void				SetParentPos(GwVector vParentPos);
	void				SetCameraPos(GwVector vCameraPos);

	void				OnPlay();
	void				OnPause();
	void				OnStop();

	void				Reset(GwVector vParentPos, GwVector vParentRot, GwVector vParentScale, 
							  GwVector vStartPos, GwVector vStartDirection, float fEmissionLifeTime, float fStartSpeed,
							  GwVector vStartScale, GwVector vStartRotation, GwColor clrStartColor, float fStartAlpha, float fGravity, bool IsLocal);

private:

	void				CreateComponent_Trans_Scale();
	void				CreateComponent_Trans_Color();
	void				CreateComponent_Trans_Alpha();
	void				CreateComponent_Trans_Rotation();

	void				OnOneEventDone_Rotation();
	void				SetRotationTransInfo(int nIndex);

	IParticleSystem	*	m_pParticleSystem;
	IParticleObject	*	m_pParticleObject;
	IGameObject		*	m_pGameObject;
	ePlayType			m_ePlayType;

	GwVector			m_vParentPos;
	GwVector			m_vParentRotation;
	GwVector			m_vParentScale;
	//GwVector			m_vCameraDir;
	GwVector			m_vCameraPos;

	GwVector			m_vPosition;
	GwVector			m_vDirection;

	float				m_fEmissionLifeTime;
	float				m_fStartSpeed;
	GwVector			m_vStartScale;
	GwVector			m_vStartRotation;
	GwColor				m_clrStartColor;
	float				m_fStartAlpha;
	float				m_fGravity;

	bool				m_IsLocal;

	float				m_fTime;

	IGwEventTransformer_Pos		*	m_pScale;
	IGwEventTransformer_Color	*	m_pColor;
	IGwEventTransformer_Scalar	*	m_pAlpha;
	IGwEventTransformer_Scalar	*	m_pRotation;

	int					m_nCurrTransRotationIndex;
	float				m_fRotationTime;
	ITransRotationInfo	*	m_pTransRotationInfo;

	class	IEventTransformCallback_Rotation;

	friend class IParticleSystem;
};