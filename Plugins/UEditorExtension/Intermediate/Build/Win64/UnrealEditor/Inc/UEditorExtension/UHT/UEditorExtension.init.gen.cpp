// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeUEditorExtension_init() {}
	static FPackageRegistrationInfo Z_Registration_Info_UPackage__Script_UEditorExtension;
	FORCENOINLINE UPackage* Z_Construct_UPackage__Script_UEditorExtension()
	{
		if (!Z_Registration_Info_UPackage__Script_UEditorExtension.OuterSingleton)
		{
			static const UECodeGen_Private::FPackageParams PackageParams = {
				"/Script/UEditorExtension",
				nullptr,
				0,
				PKG_CompiledIn | 0x00000040,
				0x8AA7D74E,
				0xE8C99834,
				METADATA_PARAMS(nullptr, 0)
			};
			UECodeGen_Private::ConstructUPackage(Z_Registration_Info_UPackage__Script_UEditorExtension.OuterSingleton, PackageParams);
		}
		return Z_Registration_Info_UPackage__Script_UEditorExtension.OuterSingleton;
	}
	static FRegisterCompiledInInfo Z_CompiledInDeferPackage_UPackage__Script_UEditorExtension(Z_Construct_UPackage__Script_UEditorExtension, TEXT("/Script/UEditorExtension"), Z_Registration_Info_UPackage__Script_UEditorExtension, CONSTRUCT_RELOAD_VERSION_INFO(FPackageReloadVersionInfo, 0x8AA7D74E, 0xE8C99834));
PRAGMA_ENABLE_DEPRECATION_WARNINGS
