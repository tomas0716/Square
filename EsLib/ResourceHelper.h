#pragma once

class IResourceHelper
{
public:
	IResourceHelper(void);
	~IResourceHelper(void);

	static IGwBillboardTex	*	Create_BillboardTex(string strMediaPath, string strFileName);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	@ strMediaPath : gb 파일이 있는 Path.
	//  @ strFileName : gb 파일명
	//	@ Output_MeshTextureInfoList : 얻어올 텍스쳐 파일 정보
	//	
	//	> 생성한 Object로 텍스쳐 정보를 셋팅함과 동시에 Output_MeshTextureInfoList 에 텍스쳐 정보를 담는다.
	//
	static IGwStaticObject	*	CreateStaticObject_AutoTexture(string strMediaPath, string strFileName, vector<ITextureFileInfo> & Output_MeshTextureInfoList);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	@ strMediaPath : gb 파일이 있는 Path.
	//  @ strFileName : gb 파일명
	//	@ pMeshTextureInfoList : 메시에 셋팅할 텍스쳐 정보
	//	
	//	> 생성한 Object에 pMeshTextureInfoList 정보대로 텍스쳐를 셋팅한다.
	//
	static IGwStaticObject	*	CreateStaticObject_SetTexture(string strMediaPath, string strFileName, vector<ITextureFileInfo> * pMeshTextureInfoList);

	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
	//	@ strMediaPath : gb 파일이 있는 Path.
	//	@ strTexturePath : 텍스쳐가 있는 Path
	//  @ strFileName : gb 파일명
	//
	//	> 입력한 정보대로 Object 를 생성
	//
	static IGwStaticObject	*	CreateStaticObject(string strMediaPath, string strTexturePath, string strFileName);
	// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

	static IGwAnimSequence	*	Create_AnimSequence(string strMediaPath, string strFileName);

	static IFileInfo			FindBoneFileInfo(string strMediaPath, string strFileName);	// 메쉬 파일명으로 본 gb 파일명을 유추해서 파일정보를 찾는다. 100% 찾는건 아님.

	static void					SetStaticObjectTexture(IGwStaticObject * pStaticObject, ITextureFileInfo * pTextureFileInfo);
	static void					SetStaticObjectTexture(IGwStaticObject * pStaticObject, vector<ITextureFileInfo> * pMeshTextureInfoList);

private:

	static void					FindFilePath(string strMediaPath, string strExceptionFolder, vector<IFileInfo> * pFileInfoList);
	static void					Recursive_FindFilePath(string strMediaPath, string strExceptionFolder, vector<IFileInfo> * pFileInfoList);
};