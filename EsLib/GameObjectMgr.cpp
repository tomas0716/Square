#include "StdAfx.h"
#include "GameObjectMgr.h"

IGameObjectMgr::IGameObjectMgr(void)
{
	m_pHierarchyNode = new IGameObject();
	m_pEmptyGameObject = new IGameObject();

	// 런타임 클래스 선언 // ITypeObject 상속받은 클래스를 Pre Compile 하기 위해서 생성후 삭제
	IGwRefObject * pRefObject = NULL;
	IType * pType = NULL;
	ITypeObject * pTypeObject = NULL;

	pRefObject = new IComponent();						pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IComponent");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IComponent_MeshCollider();			pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IComponent_MeshCollider");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IComponent_Trans();				pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IComponent_Trans");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IComponent_Trans_Scale();			pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IComponent_Trans_Scale");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IComponent_Trans_Color();			pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IComponent_Trans_Color");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IComponent_Trans_Alpha();			pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IComponent_Trans_Alpha");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IComponent_Trans_Rotation();		pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IComponent_Trans_Rotation");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IComponent_Trans_AnimSequence();	pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IComponent_Trans_AnimSequence");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new ITransInfo();						pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("ITransInfo");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new ITransScaleInfo();					pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("ITransScaleInfo");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new ITransColorInfo();					pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("ITransColorInfo");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new ITransAlphaInfo();					pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("ITransAlphaInfo");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new ITransRotationInfo();				pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("ITransRotationInfo");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new ITransSequenceAnimInfo();			pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("ITransSequenceAnimInfo");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IGameObject();						pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IGameObject");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new INode();							pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("INode");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IObject();							pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IObject");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IEmissionShape();					pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IEmissionShape");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IParticleShape_Cone();				pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IParticleShape_Cone");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IParticleShape_Sphere();			pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IParticleShape_Sphere");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IParticleShape_Box();				pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IParticleShape_Box");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IParticleShape_Mesh();				pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IParticleShape_Mesh");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IParticleObject();					pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IParticleObject");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IParticleObjectInfo();				pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IParticleObjectInfo");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IParticleObjectInfo_Billboard();	pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IParticleObjectInfo_Billboard");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IParticleObjectInfo_Mesh();		pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IParticleObjectInfo_Mesh");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IParticleObjectInfo_SkinMesh();	pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IParticleObjectInfo_SkinMesh");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IPathObject();						pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IPathObject");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new IBillboard();						pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("IBillboard");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;

	pRefObject = new ISkinMesh();						pRefObject->Release();
	pType = ITypeManager::GetInstance()->FindType("ISkinMesh");
	pTypeObject = pType->CreateInstance();	delete pTypeObject;
}

IGameObjectMgr::~IGameObjectMgr(void)
{
	m_pHierarchyNode->Release();
	m_pEmptyGameObject->Release();
}

void			IGameObjectMgr::Update(float fDeltaTime)
{
	m_pHierarchyNode->Update(fDeltaTime);
}

void			IGameObjectMgr::Render(float fDeltaTime)
{
	m_pHierarchyNode->Render(fDeltaTime);
}

void			IGameObjectMgr::AddHierarchyNode(IGameObject * pGameObject)
{
	m_pHierarchyNode->AddChild(pGameObject);
}

void			IGameObjectMgr::RemoveHierarchyNode(IGameObject * pGameObject)
{
	m_pHierarchyNode->RemoveChild(pGameObject);
}

IGameObject	*	IGameObjectMgr::GetEmptyGameObject()
{
	return m_pEmptyGameObject;
}

IGameObject	*	IGameObjectMgr::GetHierarchyNode()
{
	return m_pHierarchyNode;
}

IGameObject	*	IGameObjectMgr::LoadFile(const char * pszMediaPath, const char * pszFileName)
{
	IGwPakData * pPakData = Gw::GetRscPakMgr()->Load(pszMediaPath, pszFileName);
	if( pPakData != NULL ) 
	{
		BYTE * pData = pPakData->GetData_Byte();

		char szClassName[64] = { 0, };
		memcpy(szClassName, pData, sizeof(char) * 64);	pData += sizeof(char) * 64;

		IType * pType = ITypeManager::GetInstance()->FindType(szClassName);
		IGameObject * pGameObject = dynamic_cast<IGameObject*>(pType->CreateInstance());

		pData = pGameObject->LoadFile(pData);

		pPakData->Release();

		pGameObject->SetCalcuTransformValid(false);
		pGameObject->ReCalculationLocalMatrix();

		return pGameObject;
	}

	return NULL;
}

void			IGameObjectMgr::SaveFile(const char * pszMediaPath, const char * pszFileName, IGameObject * pGameObject)
{
	string strFullName = pszMediaPath;
	strFullName += "\\";
	strFullName += pszFileName;
	IFile kFile;
	kFile.CreateFile(strFullName.c_str(), false);

	const char * pszClassName = IHashObject::GetInstance()->GetHashString(pGameObject->GetClassHashCode());
	kFile.WriteFile((void*)pszClassName, sizeof(char) * 64);

	pGameObject->SaveFile(kFile);
}

void			IGameObjectMgr::LoadHierarchy()
{
	IGwPakData * pPakData = Gw::GetRscPakMgr()->Load("", "Hierarchy.obj");
	if( pPakData != NULL ) 
	{
		BYTE * pData = pPakData->GetData_Byte();
		pData = m_pHierarchyNode->LoadFile(pData);

		pPakData->Release();
	}
}

void			IGameObjectMgr::SaveHierarchy()
{
	IFile kFile;

	kFile.CreateFile("Hierarchy.obj", false);
	m_pHierarchyNode->SaveFile(kFile);
}
