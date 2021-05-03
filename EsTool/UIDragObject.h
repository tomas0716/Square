#pragma once

class CUIDragItemBase;

class CUIDragObject : public TGwGetInstance<CUIDragObject>
{
public:
	CUIDragObject(void);
	~CUIDragObject(void);

	void				Update(float fDeltaTime);

	void				Show(IGwUIControl * pControl, CUIDragItemBase * pDragItemBase, IGwRefObject * pRefObject);
	void				Hide();

	CUIDragItemBase	*	GetDragItemBase();
	bool				IsDraging();

private:

	void				GetMinMax(IGwUIControl * pControl, GwVector2 & vMin, GwVector2 & vMax);

	IGwUIDialog		*	m_pDialog;
	IGwUISprite		*	m_pSprite_Back;

	bool				m_IsDraging;
	CUIDragItemBase	*	m_pDragItemBase;
	IGwRefObject	*	m_pRefObject;
	GwVector2			m_vSize;
};
