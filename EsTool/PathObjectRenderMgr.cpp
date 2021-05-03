#include "StdAfx.h"
#include "PathObjectRenderMgr.h"

CPathObjectRenderMgr::CPathObjectRenderMgr() : m_IsPathDotRender(true)
{
}

CPathObjectRenderMgr::~CPathObjectRenderMgr()
{
	map<IPathObject*,CPathObjectRender*>::iterator itr = m_PathObjectTable.begin();

	for( ; itr != m_PathObjectTable.end(); ++itr )
	{
		itr->first->Release();
		itr->second->Release();
	}

	m_PathObjectTable.clear();
}

void					CPathObjectRenderMgr::Update(float fDeltaTime)
{
	map<IPathObject*,CPathObjectRender*>::iterator itr = m_PathObjectTable.begin();

	for( ; itr != m_PathObjectTable.end(); ++itr )
	{
		itr->second->Update(fDeltaTime);
	}
}

void					CPathObjectRenderMgr::Render(float fDeltaTime)
{
	map<IPathObject*,CPathObjectRender*>::iterator itr = m_PathObjectTable.begin();

	for( ; itr != m_PathObjectTable.end(); ++itr )
	{
		itr->second->Render(fDeltaTime);
	}
}

CPathObjectRender	*	CPathObjectRenderMgr::AddPathObject(IPathObject * pPathObject)
{
	map<IPathObject*,CPathObjectRender*>::iterator itr = m_PathObjectTable.find(pPathObject);

	if( itr == m_PathObjectTable.end() )
	{
		CPathObjectRender * pRender = new CPathObjectRender(pPathObject);
		m_PathObjectTable.insert(map<IPathObject*,CPathObjectRender*>::value_type(pPathObject, pRender));
		pPathObject->AddRef();

		return pRender;
	}

	return itr->second;
}

void					CPathObjectRenderMgr::RemovePathObject(IPathObject * pPathObject)
{
	map<IPathObject*,CPathObjectRender*>::iterator itr = m_PathObjectTable.find(pPathObject);

	if( itr != m_PathObjectTable.end() )
	{
		itr->first->Release();
		itr->second->Release();
		m_PathObjectTable.erase(itr);
	}
}

void					CPathObjectRenderMgr::ReloadPathObject(IPathObject * pPathObject)
{
	map<IPathObject*,CPathObjectRender*>::iterator itr = m_PathObjectTable.find(pPathObject);

	if( itr != m_PathObjectTable.end() )
	{
		itr->second->ReLoad();
	}
}

CPathObjectRender	*	CPathObjectRenderMgr::FindPathObjectRender(IPathObject * pPathObject)
{
	map<IPathObject*,CPathObjectRender*>::iterator itr = m_PathObjectTable.find(pPathObject);

	if( itr != m_PathObjectTable.end() )
	{
		return itr->second;
	}

	return NULL;
}

void					CPathObjectRenderMgr::OnPathDotRender() 
{
	m_IsPathDotRender = !m_IsPathDotRender; 

	map<IPathObject*,CPathObjectRender*>::iterator itr = m_PathObjectTable.begin();

	for( ; itr != m_PathObjectTable.end(); ++itr )
	{
		itr->second->SetPathDotRender(m_IsPathDotRender);
	}
}