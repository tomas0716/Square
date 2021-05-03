#pragma once

class CPathObjectRender : public IGwRefObject
{
public:
	CPathObjectRender(IPathObject * pPathObject);
	~CPathObjectRender();

	void					Update(float fDeltaTime);
	void					Render(float fDeltaTime);

	void					SetSelectDot(sPathDotInfo * pPathDotInfo, int nIndexPlus);
	void					RemovePathObjectControllerRender();

	void					SetPathDotRender(bool IsRender) { m_IsPathDotRender = IsRender; }

	void					ReLoad();

private:

	void					CreateMiddleDot();
	void					Clear();
	void					ClearPolyLine();

	void					OnLButtonDown_Picking_Dot(const EventArg_PickingEntity& Arg);
	void					OnRButtonDown_Picking_Dot(const EventArg_PickingEntity& Arg);

	void					OnLButtonUp_Picking_MiddleDot(const EventArg_PickingEntity& Arg);

	void					OnGizmoObjectMove(const EventArg_GizmoMove& Arg);

	void					OnObjectEditState(const EventArg_PopupMenu_Click& Arg);

	IPathObject			*	m_pPathObject;
	sPathDotInfo		*	m_pPickingPathDotInfo;

	CPopupMenu			*	m_pPopupMenu_AddDelete;
	CPopupMenu			*	m_pPopupMenu_Add;
	CPopupMenu			*	m_pPopupMenu_Delete;

	map<IGwScnObject*, sPathDotInfo*>	m_PathDotInfoTable;
	map<IGwScnObject*, int>				m_MiddleDotIndexTable;

	vector<GwVector>		m_PosList;
	IGwBillboardTex		 *	m_pPolyTex;
	vector<IGwPolyline*>	m_PolylineList;
	int						m_nActivePolyline;

	bool					m_IsPathDotRender;

	class CPathObjectControllerRender;
	CPathObjectControllerRender	*	m_pPathObjectControllerRender;
};
