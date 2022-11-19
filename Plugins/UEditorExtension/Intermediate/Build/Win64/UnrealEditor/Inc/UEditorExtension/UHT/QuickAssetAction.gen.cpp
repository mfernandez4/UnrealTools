// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "UEditorExtension/Public/QuickAssetAction.h"
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeQuickAssetAction() {}
// Cross Module References
	BLUTILITY_API UClass* Z_Construct_UClass_UAssetActionUtility();
	UEDITOREXTENSION_API UClass* Z_Construct_UClass_UQuickAssetAction();
	UEDITOREXTENSION_API UClass* Z_Construct_UClass_UQuickAssetAction_NoRegister();
	UPackage* Z_Construct_UPackage__Script_UEditorExtension();
// End Cross Module References
	DEFINE_FUNCTION(UQuickAssetAction::execTestFunction)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		UQuickAssetAction::TestFunction();
		P_NATIVE_END;
	}
	void UQuickAssetAction::StaticRegisterNativesUQuickAssetAction()
	{
		UClass* Class = UQuickAssetAction::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "TestFunction", &UQuickAssetAction::execTestFunction },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UQuickAssetAction_TestFunction_Statics
	{
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UECodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UQuickAssetAction_TestFunction_Statics::Function_MetaDataParams[] = {
		{ "CallInEditor", "true" },
		{ "Category", "QuickAssetAction" },
		{ "ModuleRelativePath", "Public/QuickAssetAction.h" },
	};
#endif
	const UECodeGen_Private::FFunctionParams Z_Construct_UFunction_UQuickAssetAction_TestFunction_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UQuickAssetAction, nullptr, "TestFunction", nullptr, nullptr, 0, nullptr, 0, RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UQuickAssetAction_TestFunction_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_UQuickAssetAction_TestFunction_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UQuickAssetAction_TestFunction()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UECodeGen_Private::ConstructUFunction(&ReturnFunction, Z_Construct_UFunction_UQuickAssetAction_TestFunction_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	IMPLEMENT_CLASS_NO_AUTO_REGISTRATION(UQuickAssetAction);
	UClass* Z_Construct_UClass_UQuickAssetAction_NoRegister()
	{
		return UQuickAssetAction::StaticClass();
	}
	struct Z_Construct_UClass_UQuickAssetAction_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UECodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UECodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UQuickAssetAction_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UAssetActionUtility,
		(UObject* (*)())Z_Construct_UPackage__Script_UEditorExtension,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UQuickAssetAction_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UQuickAssetAction_TestFunction, "TestFunction" }, // 3470212149
	};
#if WITH_METADATA
	const UECodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UQuickAssetAction_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n * \n */" },
		{ "HideCategories", "Object" },
		{ "IncludePath", "QuickAssetAction.h" },
		{ "ModuleRelativePath", "Public/QuickAssetAction.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UQuickAssetAction_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UQuickAssetAction>::IsAbstract,
	};
	const UECodeGen_Private::FClassParams Z_Construct_UClass_UQuickAssetAction_Statics::ClassParams = {
		&UQuickAssetAction::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UQuickAssetAction_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_UQuickAssetAction_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UQuickAssetAction()
	{
		if (!Z_Registration_Info_UClass_UQuickAssetAction.OuterSingleton)
		{
			UECodeGen_Private::ConstructUClass(Z_Registration_Info_UClass_UQuickAssetAction.OuterSingleton, Z_Construct_UClass_UQuickAssetAction_Statics::ClassParams);
		}
		return Z_Registration_Info_UClass_UQuickAssetAction.OuterSingleton;
	}
	template<> UEDITOREXTENSION_API UClass* StaticClass<UQuickAssetAction>()
	{
		return UQuickAssetAction::StaticClass();
	}
	DEFINE_VTABLE_PTR_HELPER_CTOR(UQuickAssetAction);
	UQuickAssetAction::~UQuickAssetAction() {}
	struct Z_CompiledInDeferFile_FID_Projects___Personal_Unreal_Projects_UnrealTools_5_1_Plugins_UEditorExtension_Source_UEditorExtension_Public_QuickAssetAction_h_Statics
	{
		static const FClassRegisterCompiledInInfo ClassInfo[];
	};
	const FClassRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Projects___Personal_Unreal_Projects_UnrealTools_5_1_Plugins_UEditorExtension_Source_UEditorExtension_Public_QuickAssetAction_h_Statics::ClassInfo[] = {
		{ Z_Construct_UClass_UQuickAssetAction, UQuickAssetAction::StaticClass, TEXT("UQuickAssetAction"), &Z_Registration_Info_UClass_UQuickAssetAction, CONSTRUCT_RELOAD_VERSION_INFO(FClassReloadVersionInfo, sizeof(UQuickAssetAction), 1125234750U) },
	};
	static FRegisterCompiledInInfo Z_CompiledInDeferFile_FID_Projects___Personal_Unreal_Projects_UnrealTools_5_1_Plugins_UEditorExtension_Source_UEditorExtension_Public_QuickAssetAction_h_2140727876(TEXT("/Script/UEditorExtension"),
		Z_CompiledInDeferFile_FID_Projects___Personal_Unreal_Projects_UnrealTools_5_1_Plugins_UEditorExtension_Source_UEditorExtension_Public_QuickAssetAction_h_Statics::ClassInfo, UE_ARRAY_COUNT(Z_CompiledInDeferFile_FID_Projects___Personal_Unreal_Projects_UnrealTools_5_1_Plugins_UEditorExtension_Source_UEditorExtension_Public_QuickAssetAction_h_Statics::ClassInfo),
		nullptr, 0,
		nullptr, 0);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
