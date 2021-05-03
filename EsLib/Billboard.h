#pragma once

class IBillboard : public IGameObject
{
	I_DECLARE_DYNCREATE(IBillboard);
public:
	IBillboard();
	IBillboard(const char * pName);
	IBillboard(const char * pName, IGwBillboard * pBillboard);
	virtual ~IBillboard();

	virtual void		Update(float fDeltaTime);
	virtual void		Render(float fDeltaTime);
	virtual	void		Render(float fDeltaTime, IGwViewFrustumOct * pFrustum);

	virtual void		SetBillboardTex(IGwBillboardTex * pBillboardTex);
	virtual void		SetBillboard(IGwBillboard * pBillboard);

	virtual void		SetRotation(GwVector vRot);

	virtual void		Reset();

	virtual BYTE	*	LoadFile(BYTE * pData);
	virtual void		SaveFile(IFile & kFile);

protected:

	IGwBillboard	*	m_pBillboard;
	float				m_fTime;
};