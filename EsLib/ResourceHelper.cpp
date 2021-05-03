#include "StdAfx.h"
#include "ResourceHelper.h"

IResourceHelper::IResourceHelper(void)
{
}

IResourceHelper::~IResourceHelper(void)
{
}

IGwBillboardTex	*	IResourceHelper::Create_BillboardTex(string strMediaPath, string strFileName)
{
	char	szMediaPathOld[STRING_MAX] = "";
	StringCchCopyA(szMediaPathOld, STRING_MAX, Gw::GetMtrlMgr()->GetMediaPath());
	Gw::GetMtrlMgr()->SetMediaPath(strMediaPath.c_str());

	IGwBillboardTex	* pTexture = Gw::GetBillboardMgr()->CreateBillboardTexture_InPak(GwNullString, strFileName.c_str());

	Gw::GetMtrlMgr()->SetMediaPath(szMediaPathOld);

	return pTexture;
}

IGwStaticObject	*	IResourceHelper::CreateStaticObject_AutoTexture(string strMediaPath, string strFileName, vector<ITextureFileInfo> & Output_MeshTextureInfoList)
{
	Output_MeshTextureInfoList.clear();

	IGwStaticObject * pStaticObject = NULL;

	char	szMediaPathOld[STRING_MAX] = "";
	StringCchCopyA(szMediaPathOld, STRING_MAX, Gw::GetMtrlMgr()->GetMediaPath());

	Gw::GetBundleMgr()->SetMediaPath(strMediaPath.c_str());
	Gw::GetBundleMgr()->SetTexturePath(strMediaPath.c_str());

	IGwBundle*	pPack;
	pPack = Gw::GetBundleMgr()->Load_InPak(strFileName.c_str());

	if(pPack)
	{
		pStaticObject = Gw::GetScnMgr()->CreateStaticObject(pPack, SHADERTECH_VS_TOONSHADEONLY, SHADERTECH_VSSHADOW);
		pPack->Release();
	}

	Gw::GetBundleMgr()->SetMediaPath(szMediaPathOld);

	if( pStaticObject != NULL )
	{
		vector<IGwRndNode*> MeshList;
		pStaticObject->FindAllTreeNodes(MeshList,GWRNDNODE_MESH);

		for( int i = 0; i < (int)MeshList.size(); ++i )
		{
			IGwBaseMesh * pBaseMesh = dynamic_cast<IGwBaseMesh*>(MeshList[i]);
			int nNumSubset = pBaseMesh->GetNumSubset();

			for( int j = 0; j < nNumSubset; ++j )
			{
				IGwMtrlSet * pMtrlSet = pBaseMesh->GetSubMtrlSetBySubset(j);

				if( pMtrlSet != NULL )
				{
					int nNumTex = pMtrlSet->GetNumTex();

					for( int k = 0; k < nNumTex; ++k )
					{
						IGwTex * pTex = pMtrlSet->GetTex(k);

						if( pTex != NULL )
						{
							ITextureFileInfo MeshTextureInfo;
							MeshTextureInfo.m_nMeshIndex = i;
							MeshTextureInfo.m_nSubMtrlSetIndex = j;
							MeshTextureInfo.m_nTextureIndex = k;
							MeshTextureInfo.m_strFileName = pTex->GetTextureFileName();

							Output_MeshTextureInfoList.push_back(MeshTextureInfo);
						}
					}
				}
			}
		}

		vector<IFileInfo> FileInfoList;
		int nCount = (int)Output_MeshTextureInfoList.size();
		for( int i = 0; i < nCount; ++i )
		{
			IFileInfo kFileInfo;
			kFileInfo.m_strFileName = Output_MeshTextureInfoList[i].m_strFileName;
			FileInfoList.push_back(kFileInfo);
		}

		if( FileInfoList.empty() == false )
			FindFilePath(strMediaPath, "", &FileInfoList);

		for( int i = 0; i < nCount; ++i )
		{
			Output_MeshTextureInfoList[i].m_strPath = FileInfoList[i].m_strPath;
		}

		SetStaticObjectTexture(pStaticObject, &Output_MeshTextureInfoList);	
	}

	return pStaticObject;
}

IGwStaticObject	*	IResourceHelper::CreateStaticObject_SetTexture(string strMediaPath, string strFileName, vector<ITextureFileInfo> * pMeshTextureInfoList)
{
	IGwStaticObject * pStaticObject = NULL;

	char	szMediaPathOld[STRING_MAX] = "";
	StringCchCopyA(szMediaPathOld, STRING_MAX, Gw::GetBundleMgr()->GetMediaPath());

	Gw::GetBundleMgr()->SetMediaPath(strMediaPath.c_str());

	IGwBundle*	pPack;
	pPack = Gw::GetBundleMgr()->Load_InPak(strFileName.c_str());

	if(pPack)
	{
		pStaticObject = Gw::GetScnMgr()->CreateStaticObject(pPack, SHADERTECH_VS_TOONSHADEONLY, SHADERTECH_VSSHADOW);
		pPack->Release();
	}

	Gw::GetBundleMgr()->SetMediaPath(szMediaPathOld);

	SetStaticObjectTexture(pStaticObject, pMeshTextureInfoList);	

	return pStaticObject;
}

IGwStaticObject	*	IResourceHelper::CreateStaticObject(string strMediaPath, string strTexturePath, string strFileName)
{
	IGwStaticObject * pStaticObject = NULL;

	char	szMediaPathOld[STRING_MAX] = "";
	char	szTexturePathOld[STRING_MAX] = "";
	StringCchCopyA(szMediaPathOld, STRING_MAX, Gw::GetBundleMgr()->GetMediaPath());
	StringCchCopyA(szTexturePathOld, STRING_MAX, Gw::GetBundleMgr()->GetTexturePath());

	Gw::GetBundleMgr()->SetMediaPath(strMediaPath.c_str());
	Gw::GetBundleMgr()->SetTexturePath(strTexturePath.c_str());

	IGwBundle*	pPack;
	pPack = Gw::GetBundleMgr()->Load_InPak(strFileName.c_str());

	if(pPack)
	{
		pStaticObject = Gw::GetScnMgr()->CreateStaticObject(pPack, SHADERTECH_VS_TOONSHADEONLY, SHADERTECH_VSSHADOW);
		pPack->Release();
	}

	Gw::GetBundleMgr()->SetMediaPath(szMediaPathOld);
	Gw::GetBundleMgr()->SetTexturePath(szTexturePathOld);

	return pStaticObject;
}

IGwAnimSequence	*	IResourceHelper::Create_AnimSequence(string strMediaPath, string strFileName)
{
	IGwAnimSequence* pAnimSequence = NULL;

	string strMediaPathOld = Gw::GetBundleMgr()->GetMediaPath();

	Gw::GetBundleMgr()->SetMediaPath(strMediaPath.c_str());

	IGwBundle* pBundle = Gw::GetBundleMgr()->Load_InPak(strFileName.c_str());
	if (pBundle != NULL)
	{
		pAnimSequence = Gw::GetAnimMgr()->CreateAnimSequence(pBundle);
		pBundle->Release();
	}		

	Gw::GetBundleMgr()->SetMediaPath(strMediaPathOld.c_str());

	return pAnimSequence;
}

IFileInfo			IResourceHelper::FindBoneFileInfo(string strMediaPath, string strFileName)
{
	string strBoneFileName = "";
	vector<IFileInfo> FileInfoList;
	IFileInfo kFileInfo;
	kFileInfo.m_strFileName = "Bone_";
	kFileInfo.m_strFileName += strFileName;
	FileInfoList.push_back(kFileInfo);

	FindFilePath(strMediaPath, "", &FileInfoList);

	if( FileInfoList[0].m_strPath == "" )
	{
	}
	else 
	{
		FileInfoList[0].m_strPath += "\\";
	}
	
	return FileInfoList[0];
}

void				IResourceHelper::SetStaticObjectTexture(IGwStaticObject * pStaticObject, ITextureFileInfo * pTextureFileInfo)
{
	vector<IGwRndNode*> MeshList;
	pStaticObject->FindAllTreeNodes(MeshList,GWRNDNODE_MESH);

	for( int i = 0; i < (int)MeshList.size(); ++i )
	{
		IGwBaseMesh * pBaseMesh = dynamic_cast<IGwBaseMesh*>(MeshList[i]);
		int nNumSubset = pBaseMesh->GetNumSubset();

		for( int j = 0; j < nNumSubset; ++j )
		{
			IGwMtrlSet * pMtrlSet = pBaseMesh->GetSubMtrlSetBySubset(j);

			if( pMtrlSet != NULL )
			{
				int nNumTex = pMtrlSet->GetNumTex();

				for( int k = nNumTex - 1; k >= 0; --k )
				{
					IGwTex * pOriginTex = pMtrlSet->GetTex(k);

					if( pTextureFileInfo->m_nMeshIndex == i && 
						pTextureFileInfo->m_nSubMtrlSetIndex == j &&
						pTextureFileInfo->m_nTextureIndex == k )
					{
						char strOldTexPath[STRING_MAX];
						StringCchCopyA(strOldTexPath,STRING_MAX,Gw::GetMtrlMgr()->GetMediaPath());

						string strMediaPath = pTextureFileInfo->m_strPath;
						strMediaPath += "\\";
						Gw::GetMtrlMgr()->SetMediaPath(strMediaPath.c_str());
						IGwTex* pTex = Gw::GetMtrlMgr()->CreateTex_InPak(pTextureFileInfo->m_strFileName.c_str(), "");
						Gw::GetMtrlMgr()->SetMediaPath(strOldTexPath);

						pTex->SetTexType(pOriginTex->GetTexType());
						pTex->SetMappingChannelIdx(pOriginTex->GetMappingChannelIdx());
						pTex->SetAmount(pOriginTex->GetAmount());

						pMtrlSet->SetTex(k, pTex);	pTex->Release();
					}
				}
			}
		}
	}
}

void				IResourceHelper::SetStaticObjectTexture(IGwStaticObject * pStaticObject, vector<ITextureFileInfo> * pMeshTextureInfoList)
{
	vector<IGwRndNode*> MeshList;
	pStaticObject->FindAllTreeNodes(MeshList,GWRNDNODE_MESH);

	for( int i = 0; i < (int)MeshList.size(); ++i )
	{
		IGwBaseMesh * pBaseMesh = dynamic_cast<IGwBaseMesh*>(MeshList[i]);
		int nNumSubset = pBaseMesh->GetNumSubset();

		for( int j = 0; j < nNumSubset; ++j )
		{
			IGwMtrlSet * pMtrlSet = pBaseMesh->GetSubMtrlSetBySubset(j);

			if( pMtrlSet != NULL )
			{
				int nNumTex = pMtrlSet->GetNumTex();

				for( int k = nNumTex - 1; k >= 0; --k )
				{
					IGwTex * pOriginTex = pMtrlSet->GetTex(k);

					vector<ITextureFileInfo>::iterator itr = pMeshTextureInfoList->begin();

					for( ; itr != pMeshTextureInfoList->end(); ++itr )
					{
						ITextureFileInfo * pMeshTextureInfo = &(*itr);
						if( pMeshTextureInfo->m_nMeshIndex == i && 
							pMeshTextureInfo->m_nSubMtrlSetIndex == j &&
							pMeshTextureInfo->m_nTextureIndex == k )
						{
							char strOldTexPath[STRING_MAX];
							StringCchCopyA(strOldTexPath,STRING_MAX,Gw::GetMtrlMgr()->GetMediaPath());

							string strMediaPath = pMeshTextureInfo->m_strPath;
							strMediaPath += "\\";
							Gw::GetMtrlMgr()->SetMediaPath(strMediaPath.c_str());
							IGwTex* pTex = Gw::GetMtrlMgr()->CreateTex_InPak(pMeshTextureInfo->m_strFileName.c_str(), "");
							Gw::GetMtrlMgr()->SetMediaPath(strOldTexPath);

							pTex->SetTexType(pOriginTex->GetTexType());
							pTex->SetMappingChannelIdx(pOriginTex->GetMappingChannelIdx());
							pTex->SetAmount(pOriginTex->GetAmount());

							pMtrlSet->SetTex(k, pTex);	pTex->Release();
						}
					}
				}
			}
		}
	}
}

void				IResourceHelper::FindFilePath(string strMediaPath, string strExceptionFolder, vector<IFileInfo> * pFileInfoList)
{
	string strOrigToken = CGwStrTok::GetTokenFormat();
	CGwStrTok::SetTokenFormat("\\");
	CGwStrTok::SetString(strMediaPath.c_str());
	CGwStrTok::SetTokenFormat(strOrigToken.c_str());

	int nTokenNum = CGwStrTok::GetNumToken();
	string strFolderName = CGwStrTok::GetToken(nTokenNum-1);
	string strSubMediaPath = strMediaPath.substr(0, strMediaPath.size() - 1);
	Recursive_FindFilePath(strSubMediaPath, strExceptionFolder, pFileInfoList);

	size_t pos = strMediaPath.find(strFolderName.c_str());
	string TopLevelFolder = strMediaPath.substr(0, pos);

	bool IsAllFind = true;

	vector<IFileInfo>::iterator itr = pFileInfoList->begin();

	for( ; itr != pFileInfoList->end(); ++itr )
	{
		if( (*itr).m_strPath == "" )
		{
			IsAllFind = false;
			break;
		}
	}

	if( IsAllFind == false )
	{
		if( strcmp(strFolderName.c_str(), "MediaPath") != 0 )
			FindFilePath(TopLevelFolder, strFolderName, pFileInfoList);
	}
}

void				IResourceHelper::Recursive_FindFilePath(string strMediaPath, string strExceptionFolder, vector<IFileInfo> * pFileInfoList)
{
	IFile kFile;

	vector<IFileInfo>::iterator itr = pFileInfoList->begin();

	for( ; itr != pFileInfoList->end(); ++itr )
	{
		string strFind = strMediaPath;
		strFind += "\\";
		strFind += (*itr).m_strFileName;
		if( (*itr).m_strPath == "" && kFile.CheckFile(strFind.c_str()) == true )
		{
			(*itr).m_strPath = strMediaPath;
		}
	}

	char				szDirectoryFile[MAX_PATH] = "";
	HANDLE				search_handle;
	WIN32_FIND_DATAA	file_data;

	char szTargetDir[MAX_PATH] = "";
	strcpy ( szTargetDir , strMediaPath.c_str() );
	strcat ( szTargetDir , "\\*.*" );

	search_handle = FindFirstFileA ( szTargetDir , &file_data );

	if( INVALID_HANDLE_VALUE != search_handle )
	{
		do{
			if(  (FILE_ATTRIBUTE_DIRECTORY & file_data.dwFileAttributes ) )
			{
				if( strcmp ( file_data.cFileName, "." ) == 0 || 
					strcmp ( file_data.cFileName, ".." ) == 0 || 
					strcmp ( file_data.cFileName, ".svn" ) == 0 )			continue;
				if ( file_data.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )	continue;

				if( strExceptionFolder != file_data.cFileName )
				{
					string strChildFoldPath = strMediaPath;
					strChildFoldPath += "\\";
					strChildFoldPath += file_data.cFileName;
					Recursive_FindFilePath(strChildFoldPath, "", pFileInfoList);
				}
			}
		}while( FindNextFileA ( search_handle, &file_data ) );

		FindClose(search_handle);
	}
}