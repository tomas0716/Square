#pragma once

class CPathObjectRenderMgr : public TGwGetInstance<CPathObjectRenderMgr>
{
public:
	CPathObjectRenderMgr();
	~CPathObjectRenderMgr();

	void					Update(float fDeltaTime);
	void					Render(float fDeltaTime);

	CPathObjectRender	*	AddPathObject(IPathObject * pPathObject);
	void					RemovePathObject(IPathObject * pPathObject);
	
	void					ReloadPathObject(IPathObject * pPathObject);

	CPathObjectRender	*	FindPathObjectRender(IPathObject * pPathObject);

	void					OnPathDotRender();

private:
	map<IPathObject*,CPathObjectRender*>		m_PathObjectTable;

	bool					m_IsPathDotRender;
};
