#pragma once

class IResourceHelper
{
public:
	IResourceHelper(void);
	~IResourceHelper(void);

	static IGwBillboardTex	*	Create_BillboardTex(string strMediaPath, string strFileName);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	@ strMediaPath : gb ������ �ִ� Path.
	//  @ strFileName : gb ���ϸ�
	//	@ Output_MeshTextureInfoList : ���� �ؽ��� ���� ����
	//	
	//	> ������ Object�� �ؽ��� ������ �����԰� ���ÿ� Output_MeshTextureInfoList �� �ؽ��� ������ ��´�.
	//
	static IGwStaticObject	*	CreateStaticObject_AutoTexture(string strMediaPath, string strFileName, vector<ITextureFileInfo> & Output_MeshTextureInfoList);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	@ strMediaPath : gb ������ �ִ� Path.
	//  @ strFileName : gb ���ϸ�
	//	@ pMeshTextureInfoList : �޽ÿ� ������ �ؽ��� ����
	//	
	//	> ������ Object�� pMeshTextureInfoList ������� �ؽ��ĸ� �����Ѵ�.
	//
	static IGwStaticObject	*	CreateStaticObject_SetTexture(string strMediaPath, string strFileName, vector<ITextureFileInfo> * pMeshTextureInfoList);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	@ strMediaPath : gb ������ �ִ� Path.
	//	@ strTexturePath : �ؽ��İ� �ִ� Path
	//  @ strFileName : gb ���ϸ�
	//
	//	> �Է��� ������� Object �� ����
	//
	static IGwStaticObject	*	CreateStaticObject(string strMediaPath, string strTexturePath, string strFileName);
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	static IGwAnimSequence	*	Create_AnimSequence(string strMediaPath, string strFileName);

	static IFileInfo			FindBoneFileInfo(string strMediaPath, string strFileName);	// �޽� ���ϸ����� �� gb ���ϸ��� �����ؼ� ���������� ã�´�. 100% ã�°� �ƴ�.

	static void					SetStaticObjectTexture(IGwStaticObject * pStaticObject, ITextureFileInfo * pTextureFileInfo);
	static void					SetStaticObjectTexture(IGwStaticObject * pStaticObject, vector<ITextureFileInfo> * pMeshTextureInfoList);

private:

	static void					FindFilePath(string strMediaPath, string strExceptionFolder, vector<IFileInfo> * pFileInfoList);
	static void					Recursive_FindFilePath(string strMediaPath, string strExceptionFolder, vector<IFileInfo> * pFileInfoList);
};