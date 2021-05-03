#pragma once

class IParticleObject;

class IParticleSystem : public IGwRefObject
{
private:
	IParticleSystem(IParticleObject * pParticleObject);
	~IParticleSystem(void);

	void				UpdateRemove();
	void				Update(float fDeltaTime);
	void				Render(float fDeltaTime);

	void				ResetProperty();

	void				OnPlay();
	void				OnPause();
	void				OnStop();
	void				OnForceStop();		// 이미 나와있는 파티클들은 모두 사라질때까지 운동은 시킨다.

	void				CreateParticle();
	IParticle		*	GetDeadParticle();
	void				InValidParticle(IParticle * pParticle);

	void				GetStartInfo(GwVector &vPosition, GwVector &vDirection);

public:

	int					GetNumParticle();

private:

	float				m_fLifeTime;
	float				m_fEmissionRateTime;
	int					m_iEmissionRate;
	float				m_fStartDelay;
	bool				m_IsLocal;

	ePlayType			m_ePlayType;
	IParticleObject	*	m_pParticleObject;

	vector<IParticle*>	m_ParticleList;
	vector<IParticle*>	m_DeadParticleList;

	vector<IParticle*>	m_RemoveParticleList;

	float				m_fTime;
	float				m_fPrevEmissionTime;

	bool				m_ForceStop;

	friend class IParticleObject;
	friend class IParticle;
};
