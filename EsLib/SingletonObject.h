#pragma once

class ISingletonObject
{
public:

	static void				DestroyTypeModul();

	static void				Initialize();

	static void				SetTool();
	static bool				IsTool();

	static void				CreateTypeManager();
	static ITypeModule	*	GetTypeModule();

private:

	static ITypeManager	*	ms_pTypeManager;
	static ITypeModule	*	ms_pTypeModul;
	static bool				ms_Init;
	static bool				ms_IsTool;
};