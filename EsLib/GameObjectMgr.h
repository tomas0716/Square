#pragma once

class IGameObjectMgr : public TGwGetInstance<IGameObjectMgr>
{
public:
	IGameObjectMgr(void);
	~IGameObjectMgr(void);

	void			Update(float fDeltaTime);
	void			Render(float fDeltaTime);
	
	void			AddHierarchyNode(IGameObject * pGameObject);
	void			RemoveHierarchyNode(IGameObject * pGameObject);

	IGameObject	*	GetEmptyGameObject();
	IGameObject	*	GetHierarchyNode();

public:

	IGameObject	*	LoadFile(const char * pszMediaPath, const char * pszFileName);
	void			SaveFile(const char * pszMediaPath, const char * pszFileName, IGameObject * pGameObject);

	void			LoadHierarchy();
	void			SaveHierarchy();
	
private:

	IGameObject	*	m_pHierarchyNode;
	IGameObject	*	m_pEmptyGameObject;
};
