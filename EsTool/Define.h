#pragma once

const unsigned int g_nWindowModeWidth  = 1900;
const unsigned int g_nWindowModeHeight = 1050;

enum eDragObjectType
{
	eDragObject_None,
	eDragObject_TreeViewItem,
	eDragObject_ListViewItem,
	eDragObject_UISprite,
};

enum eDragObjectSender
{
	eDragObjectSender_None,
	eDragObjectSender_Hierarchy,
	eDragObjectSender_Project_File,
	eDragObjectSender_UISprite,
};

enum eDragObjectReceiver
{
	eDragObjectReceiver_None,
	eDragObjectReceiver_Hierarchy,
	eDragObjectReceiver_Project_Folder,
	eDragObjectReceiver_Project_File,
	eDragObjectReceiver_MeshPanel,
	eDragObjectReceiver_TexturePanel,
	eDragObjectReceiver_AnimPanel,
};

enum eObjectEditState
{
	eObjectEditState_None				= 0x00000000L,
	eObjectEditState_Position			= 0x80000000L,
	eObjectEditState_Rotation			= 0x40000000L,
	eObjectEditState_UpDown				= 0x20000000L,
	eObjectEditState_Delete				= 0x10000000L,

	eObjectEditState_PathObject_Add		= 0x08000000L,
	eObjectEditState_PathObject_Delete	= 0x04000000L,

	eObjectEditState_Max,
};