#pragma once

class IObject : public INode
{
	I_DECLARE_DYNCREATE(IObject);
public:
	IObject();
	IObject(const char * pName);
	virtual ~IObject();

	virtual void		Update(float fDeltaTime);
	virtual void		Render(float fDeltaTime);
	virtual	void		Render(float fDeltaTime, IGwViewFrustumOct * pFrustum);

	virtual void		SetPosition(GwVector vPos);
	virtual GwVector	GetPosition();

	virtual void		SetRotationAxis(GwVector vAxis);
	virtual GwVector	GetRotationAxis();
	virtual void		SetRotation(GwVector vRot);
	virtual GwVector	GetRotation();

	virtual void		SetScale(GwVector vScale);
	virtual GwVector	GetScale();

	virtual GwVector	GetWorldPos();
	virtual GwVector	GetWorldRot();
	virtual GwVector	GetWorldScale();

	virtual GwMat		GetWorldMat();

	virtual	void		SetVisible(bool IsVisible);
	virtual bool		IsVisible();

	virtual	void		SetVisible_ClientControl(bool IsVisible);
	virtual bool		IsVisible_ClientControl();

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

	void				ReCalculationLocalMatrix();
	void				SetCalcuTransformValid(bool IsValid);

	static GwVector		GetRotation(GwMatA & matRot);

protected:
	
	GwVector			m_vPosition;
	GwVector			m_vRotAxis;
	GwVector			m_vRotation;
	GwVector			m_vScale;

	GwMat				m_matLocalWorld;

	bool				m_IsVisible;
	bool				m_IsVisible_ClientControl;

	bool				m_IsCalcuTransformValid;
};