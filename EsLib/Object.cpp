#include "stdafx.h"
#include "Object.h"

I_IMPLEMENT_DYNCREATE(IObject, ISingletonObject::GetTypeModule())

IObject::IObject()
 : m_IsCalcuTransformValid(false), m_IsVisible(true), m_IsVisible_ClientControl(true), m_vPosition(GwVector(0,0,0)), m_vRotation(GwVector(0,0,0)), m_vScale(GwVector(1,1,1)), m_vRotAxis(GwVector(0,0,1))
{
}

IObject::IObject(const char * pName)
: INode(pName), m_IsCalcuTransformValid(false), m_IsVisible(true), m_IsVisible_ClientControl(true), m_vPosition(GwVector(0,0,0)), m_vRotation(GwVector(0,0,0)), m_vScale(GwVector(1,1,1)), m_vRotAxis(GwVector(0,0,1))
{
	SetName(pName);
}

IObject::~IObject()
{
}

void		IObject::Update(float fDeltaTime)
{
	INode::Update(fDeltaTime);
}

void		IObject::Render(float fDeltaTime)
{
	INode::Render(fDeltaTime);
}

void		IObject::Render(float fDeltaTime, IGwViewFrustumOct * pFrustum)
{
	INode::Render(fDeltaTime, pFrustum);
}

void		IObject::SetPosition(GwVector vPos)
{
	m_vPosition = vPos;
	SetCalcuTransformValid(false);
}

GwVector	IObject::GetPosition()
{
	return m_vPosition;
}

void		IObject::SetRotationAxis(GwVector vAxis)
{
	m_vRotAxis = vAxis;
}

GwVector	IObject::GetRotationAxis()
{
	return m_vRotAxis;
}

void		IObject::SetRotation(GwVector vRot)
{
	m_vRotation = GwVector(D3DXToRadian(vRot.x), D3DXToRadian(vRot.y), D3DXToRadian(vRot.z));
	SetCalcuTransformValid(false);
}

GwVector	IObject::GetRotation()
{
	return GwVector(D3DXToDegree(m_vRotation.x), D3DXToDegree(m_vRotation.y), D3DXToDegree(m_vRotation.z));
}

void		IObject::SetScale(GwVector vScale)
{
	m_vScale = vScale;
	SetCalcuTransformValid(false);
}

GwVector	IObject::GetScale()
{
	return m_vScale;
}

GwVector	IObject::GetWorldPos()
{
	ReCalculationLocalMatrix();

	GwMat matWorld;

	if( GetParent() != NULL )
	{
		IObject * pParentObject = dynamic_cast<IObject*>(GetParent());

		if( pParentObject != NULL )
		{
			GwMat matParentWorld = pParentObject->GetWorldMat();
			D3DXMatrixMultiply(&matWorld, &m_matLocalWorld, &matParentWorld);
			return GwVector(matWorld._41, matWorld._42, matWorld._43);
		}
	}

	return m_vPosition;
}

GwVector	IObject::GetWorldRot()
{
	ReCalculationLocalMatrix();

	if( GetParent() != NULL )
	{
		IObject * pParentObject = dynamic_cast<IObject*>(GetParent());

		if( pParentObject != NULL )
		{
			// 맞는지 모르겠다.
			GwVector vParentRot = pParentObject->GetWorldRot();
			return vParentRot + GwVector(D3DXToDegree(m_vRotation.x),D3DXToDegree(m_vRotation.y),D3DXToDegree(m_vRotation.z));
		}
	}

	return GwVector(D3DXToDegree(m_vRotation.x),D3DXToDegree(m_vRotation.y),D3DXToDegree(m_vRotation.z));
}

GwVector	IObject::GetWorldScale()
{
	ReCalculationLocalMatrix();

	if( GetParent() != NULL )
	{
		IObject * pParentObject = dynamic_cast<IObject*>(GetParent());

		if( pParentObject != NULL )
		{
			GwVector vParentScale = pParentObject->GetWorldScale();
			GwVector vWorldScale = GwVector(vParentScale.x * m_vScale.x,vParentScale.y * m_vScale.y,vParentScale.z * m_vScale.z);
			return vWorldScale;
		}
	}

	return m_vScale;
}

GwMat		IObject::GetWorldMat()
{
	ReCalculationLocalMatrix();

	if( GetParent() != NULL )
	{
		IObject * pParentObject = dynamic_cast<IObject*>(GetParent());

		if( pParentObject != NULL )
		{
			GwMat matParentWorld = pParentObject->GetWorldMat();
			GwMat matWorld;
			D3DXMatrixMultiply(&matWorld, &m_matLocalWorld, &matParentWorld);

			return matWorld;
		}
	}

	return m_matLocalWorld;
}

void		IObject::ReCalculationLocalMatrix()
{
	if(m_IsCalcuTransformValid == false)
	{
		m_IsCalcuTransformValid = true;

		int nNumChild = GetNumChild();

		for( int i = 0; i < nNumChild; ++i )
		{
			IObject * pObject = dynamic_cast<IObject*>(GetChild_byIndex(i));

			if( pObject != NULL ) 
			{
				pObject->ReCalculationLocalMatrix();
			}
		}

		m_matLocalWorld = IHelper::GetMatrix(m_vPosition, m_vRotation, m_vScale);
	}
}

void		IObject::SetCalcuTransformValid(bool IsValid)
{
	m_IsCalcuTransformValid = IsValid;

	int nNumChild = GetNumChild();

	for( int i = 0; i < nNumChild; ++i )
	{
		IObject * pObject = dynamic_cast<IObject*>(GetChild_byIndex(i));

		if( pObject != NULL ) 
		{
			pObject->SetCalcuTransformValid(IsValid);
		}
	}
}

void		IObject::SetVisible(bool IsVisible)
{
	m_IsVisible = IsVisible;
}

bool		IObject::IsVisible()
{
	if( GetParent() != NULL && m_IsVisible == true )
	{
		IObject * pParentObject = dynamic_cast<IObject*>(GetParent());

		if( pParentObject->IsVisible() == false )
		{
			return false;
		}
	}

	return m_IsVisible;
}

void		IObject::SetVisible_ClientControl(bool IsVisible)
{
	m_IsVisible_ClientControl = IsVisible;
}

bool		IObject::IsVisible_ClientControl()
{
	return m_IsVisible_ClientControl;
}

BYTE	*	IObject::LoadFile(BYTE * pData)
{
	pData = INode::LoadFile(pData);
	
	memcpy(&m_vPosition, pData, sizeof(GwVector));	pData += sizeof(GwVector);
	memcpy(&m_vRotation, pData, sizeof(GwVector));	pData += sizeof(GwVector);
	memcpy(&m_vScale, pData, sizeof(GwVector));		pData += sizeof(GwVector);
	memcpy(&m_IsVisible, pData, sizeof(bool));		pData += sizeof(bool);

	return pData;
}

void		IObject::SaveFile(IFile & kFile)
{
	INode::SaveFile(kFile);

	kFile.WriteFile(&m_vPosition, sizeof(GwVector));
	kFile.WriteFile(&m_vRotation, sizeof(GwVector));
	kFile.WriteFile(&m_vScale, sizeof(GwVector));
	kFile.WriteFile(&m_IsVisible, sizeof(bool));
}

GwVector	IObject::GetRotation(GwMatA & matRot)
{
    float sy = sqrt(matRot._11 * matRot._11 +  matRot._21 * matRot._21 );
 
    bool singular = sy < 1e-6;
 
    float x, y, z;
    if (!singular)
    {
        x = atan2(matRot._32 , matRot._33);
        y = atan2(-matRot._31, sy);
        z = atan2(matRot._21, matRot._11);
    }
    else
    {
        x = atan2(-matRot._23, matRot._22);
        y = atan2(-matRot._31, sy);
        z = 0;
    }
    return GwVector(D3DXToDegree(x), D3DXToDegree(y), D3DXToDegree(z));
}