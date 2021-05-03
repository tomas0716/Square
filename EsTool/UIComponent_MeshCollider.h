#pragma once

class CUIComponent_MeshCollider : public CUIComponent
{
	I_DECLARE_DYNCREATE(CUIComponent_MeshCollider);
public:
	CUIComponent_MeshCollider();
	~CUIComponent_MeshCollider(void);

	virtual void		Initialize();

	virtual void		Update(float fDeltaTime);

	void				OnButtonClick_Mesh_Find(IFileInfo * pFileInfo);
	void				OnButtonClick_Mesh_Remove();

	void				OnCheckBox_Active(bool IsChecked);

private:

	void				OnDragReceive(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender);

	CUIDragItemSprite			*	m_pUIDragItemSprite;
	IGwUIStatic					*	m_pStatic_MeshFileName;
	IGwUICheckbox				*	m_pCheckBox_CameraRayCast;

	class CBtnCallback_Mesh_Find;
	class CBtnCallback_Mesh_Remove;
	class CCheckboxCallback_Active;
};