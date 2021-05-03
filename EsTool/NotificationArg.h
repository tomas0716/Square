#pragma once

#include "NotificationCenter.h"

class CUITreeView;
class NotificationArg_TreeView : public NotificationArg
{
public:
	DeclareRTTIObject();

	CUITreeView * m_pTreeView;
};

class CUIListView;
class NotificationArg_ListView : public NotificationArg
{
public:
	DeclareRTTIObject();

	CUIListView * m_pListView;
};

class NotificationArg_GameObject_Position : public NotificationArg
{
public:
	DeclareRTTIObject();

	GwVector	m_vPosition;
};

class NotificationArg_GameObject_Rotation : public NotificationArg
{
public:
	DeclareRTTIObject();

	GwVector	m_vRotation;
};

class NotificationArg_GameObject_Scale : public NotificationArg
{
public:
	DeclareRTTIObject();

	GwVector	m_vScale;
};

class NotificationArg_InspectorComponent_TextureInfo_Find : public NotificationArg
{
public:
	DeclareRTTIObject();

	ITextureFileInfo * m_pTextureFileInfo;
};

class NotificationArg_InspectorComponent_AnimInfo_Find : public NotificationArg
{
public:
	DeclareRTTIObject();

	IFileInfo * m_pFileInfo;
};

class NotificationArg_InspectorComponent_MeshInfo_Find : public NotificationArg
{
public:
	DeclareRTTIObject();

	IFileInfo * m_pFileInfo;
};

class NotificationArg_InspectorComponent_GameObject_Tag_Remove : public NotificationArg
{
public:
	DeclareRTTIObject();

	int			m_nDepth;
};

class NotificationArg_InspectorComponent_PathObject_InfoChange : public NotificationArg
{
public:
	DeclareRTTIObject();

	sPathDotInfo * m_pPathDotInfo;
};
