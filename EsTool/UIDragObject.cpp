#include "StdAfx.h"
#include "UIDragObject.h"

CUIDragObject::CUIDragObject(void)
: m_pDragItemBase(NULL), m_IsDraging(false)
{
	m_pDialog = Gw::GetUIControlMgr()->CreateUIDialog();

	Gw::GetBillboardMgr()->BeginTextureMerge();
	Gw::GetMtrlMgr()->SetMediaPath("EsTool_Resources\\UI\\DragObject\\");
	m_pSprite_Back = CUIHelper::CreateUISprite("DragObject_Back.png");
	Gw::GetBillboardMgr()->EndTextureMerge();

	m_pRefObject = new IGwRefObject();
}

CUIDragObject::~CUIDragObject(void)
{
	Gw::GetUIControlMgr()->RemoveDialog(m_pDialog);
	m_pDialog->Release();
	m_pSprite_Back->Release();
	m_pRefObject->Release();
}

void				CUIDragObject::Update(float fDeltaTime)
{
	GwVector vPos;
	vPos.x = Gw::GetKeyMapper()->GetCursorPos()->x;
	vPos.y = Gw::GetKeyMapper()->GetCursorPos()->y;
	vPos.z = 0;

	m_pDialog->SetWorldPos(vPos);

	if( Gw::GetKeyMapper()->IsKeyUp(VK_LBUTTON) == TRUE )
	{
		Hide();
	}
}

void				CUIDragObject::Show(IGwUIControl * pControl, CUIDragItemBase * pDragItemBase, IGwRefObject * pRefObject)
{
	Hide();
	
	m_pDragItemBase = pDragItemBase;

	m_pRefObject->Release();
	m_pRefObject = pRefObject;
	m_pRefObject->AddRef();

	while(ShowCursor(false)>=0);

	IGwUIControl * pClone = pControl->Clone(true);
	GwVector2 vMin = GwVector2(FLT_MAX,FLT_MAX);
	GwVector2 vMax = GwVector2(-FLT_MAX,-FLT_MAX);
	GetMinMax(pClone, vMin, vMax);
	GwVector2 vSize = vMax - vMin;
	pClone->SetLocalPos(GwVector2(2,2));
	m_pSprite_Back->SetSize_NormalState(vSize + GwVector2(4,4));
	m_pSprite_Back->SetLocalPos((vSize + GwVector2(2,2)) * -0.5f);
	m_pSprite_Back->AddControl(pClone);	pClone->Release();
	m_pDialog->AddControl(m_pSprite_Back);

	Gw::GetUIControlMgr()->AddModalDialog(m_pDialog);

	m_IsDraging = true;
}

void				CUIDragObject::Hide()
{
	m_pRefObject->Release();
	m_pRefObject = new IGwRefObject();

	ShowCursor(true); 

	m_pSprite_Back->RemoveAllChildControl();
	m_pDialog->RemoveControl(m_pSprite_Back);
	Gw::GetUIControlMgr()->RemoveDialog(m_pDialog);
	if( m_pDialog->GetNumChildControl() != 0 )
	{
		IGwUIControl *	pControl = m_pDialog->GetFirstChildControl();

		if( pControl != NULL ) 
		{
			m_pDialog->RemoveControl(pControl);
		}
	}

	m_IsDraging = false;
}

CUIDragItemBase	*	CUIDragObject::GetDragItemBase()
{
	return m_pDragItemBase;
}

bool				CUIDragObject::IsDraging()
{
	return m_IsDraging;
}

void				CUIDragObject::GetMinMax(IGwUIControl * pControl, GwVector2 & vMin, GwVector2 & vMax)
{
	GwVector vWorldPos = *pControl->GetWorldPos();
	GwVector2 vCollisSize = *pControl->GetCollisSize();

	if( vMin.x > vWorldPos.x ) vMin.x = vWorldPos.x;
	if( vMin.y > vWorldPos.y ) vMin.y = vWorldPos.y;

	if( vMax.x < vWorldPos.x + vCollisSize.x ) vMax.x = vWorldPos.x + vCollisSize.x;
	if( vMax.y < vWorldPos.y + vCollisSize.y ) vMax.y = vWorldPos.y + vCollisSize.y;

	size_t	nNumChild = pControl->GetNumChildControl();

	IGwUIControl * pChild = NULL;
	if( nNumChild != 0 )
	{
		pChild = pControl->GetFirstChildControl();
		GetMinMax(pChild, vMin, vMax);

		for( int i = 0; i < nNumChild - 1; ++i )
		{
			pChild = pControl->GetNextChildControl(pChild);
			GetMinMax(pChild, vMin, vMax);
		}
	}
}