#pragma once

class CPickingObjectWireFrameRender : public TGwGetInstance<CPickingObjectWireFrameRender>
{
public:
	CPickingObjectWireFrameRender(void);
	~CPickingObjectWireFrameRender(void);

	void	Update(float fDeltaTime);
	void	Render(float fDeltaTime);

	void	SetSelGameObject(IGameObject * pGameObject);

	void	OnUpdate()	{ m_IsUpdate = true; }
	void	OnRender()	{ m_IsRender = !m_IsRender; }

private:

	void	ClearAllDynPolygon();

	// 일단은 하나만 한다.
	IGameObject			*	m_pSelGameObject;

	vector<IGwDynPolygon*>	m_DynPolygonList;

	bool					m_IsUpdate;
	bool					m_IsRender;
};
