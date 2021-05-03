#pragma once

class CUIComponent_SkinMeshObject : public CUIComponent
{
	I_DECLARE_DYNCREATE(CUIComponent_SkinMeshObject);
public:
	CUIComponent_SkinMeshObject(void);
	~CUIComponent_SkinMeshObject(void);

	virtual void		Initialize();

	virtual void		Update(float fDeltaTime);

	void				OnButtonClick_Texture_Find(ITextureFileInfo * pTextureFileInfo);
	void				OnButtonClick_Anim_Find(IFileInfo * pFileInfo);
	void				OnButtonClick_Anim_Remove(int nIndex);

private:

	void				RemoveAllAnimSlot();
	void				ResetAnimSlot();

	void				OnDragReceive(CUIDragItemBase * pSrcDragItem, CUIDragItemBase * pDestDragItem, eDragObjectSender eSender);

	map<CUIDragItemSprite*,IGwUIButton*>	m_DragItemSpriteTable_TextureInfo;
	map<ITextureFileInfo*,IGwUIStatic*>		m_TextureFileInfoUIStaticTable;

	map<CUIDragItemSprite*,IGwUIButton*>	m_DragItemSpriteTable_AnimInfo;
	map<IFileInfo*,IGwUIStatic*>			m_AnimFileInfoUIStaticTable;
	map<CUIDragItemSprite*,int>				m_DragItemSpriteTable_Index;
	map<int,IGwUIButton*>					m_AnimInfo_MinusButtonTable;

	IFileInfo							*	m_pEmptyFileInfo;

	int										m_nAnimSlotFirstHeight;

	bool									m_IsAnimRemoveAndReset;
	int										m_nAnimRemoveAndResetIndex;

	class CBtnCallback_Texture_Find;
	class CBtnCallback_Anim_Find;
	class CBtnCallback_Anim_Remove;
};