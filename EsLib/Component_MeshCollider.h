#pragma once

class IComponent_MeshCollider : public IComponent
{
	I_DECLARE_DYNCREATE(IComponent_MeshCollider);
public:
	IComponent_MeshCollider();
	IComponent_MeshCollider(IGameObject * pParent);
	virtual ~IComponent_MeshCollider();

	virtual	void			Update(float fDeltaTime);
	virtual void			Render(float fDeltaTime);

	virtual void			SetMeshObject(IGwStaticObject * pStaticObject, IFileInfo & kFileInfo);
	IGwStaticObject		*	GetMeshObject() { return m_pMeshObject; }

	IFileInfo			*	GetFileInfo()	{ return &m_FileInfo_Mesh; }

	void					SetCameraRayCast(bool IsRayCast);
	bool					IsCameraRayCast();

	void					CalcAABB();
	GwAABB				*	GetAABB();

	const GwOBB			*	GetOBB();

	virtual BYTE		*	LoadFile(BYTE * pData);
	virtual void			SaveFile(IFile & kFile);

protected:

	IGwStaticObject		*	m_pMeshObject;
	IFileInfo				m_FileInfo_Mesh;

	const GwOBB			*	m_pOBB;
	GwAABB					m_AABB;

	bool					m_IsCameraRayCast;
};
