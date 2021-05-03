#pragma once

class CPickingMeshColliderWireFrameRender : public TGwGetInstance<CPickingMeshColliderWireFrameRender>
{
public:
	CPickingMeshColliderWireFrameRender(void);
	~CPickingMeshColliderWireFrameRender(void);

	void	Update(float fDeltaTime);
	void	Render(float fDeltaTime);

	void	SetSelGameObject(IGameObject * pGameObject);

	void	OnUpdate()	{ m_IsUpdate = true; }
	void	OnRender()	{ m_IsRender = !m_IsRender; }

private:

	void	ClearAllDynPolygon();

	IGameObject			*	m_pSelGameObject;

	vector<IGwDynPolygon*>	m_DynPolygonList;

	bool					m_IsShowOriginMesh;

	bool					m_IsUpdate;
	bool					m_IsRender;
};
