// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"

#include "Animation/AnimBlueprint.h"
#include "Animation/BlendSpace.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraShakeBase.h"
#include "Curves/CurveLinearColor.h"
#include "Curves/CurveLinearColorAtlas.h"
#include "Engine/CurveTable.h"
#include "Engine/DataTable.h"
#include "Engine/Font.h"
#include "Engine/TextureCube.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialParameterCollection.h"
#include "NiagaraEmitter.h"
#include "NiagaraSystem.h"
#include "Particles/ParticleEmitter.h"
#include "Particles/ParticleSystem.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"

#include "QuickAssetAction.generated.h"


/**
 * 
 */
UCLASS()
class UEDITOREXTENSION_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()

public:

	/**
	 * Duplicate the selected assets a specified number of times in the same folder as the original asset.
	 * @param NumOfDuplicates - The number of times to duplicate the selected assets.
	 */
	UFUNCTION(CallInEditor, Category = "QuickAssetAction")
	static void DuplicateAssets( int32 NumOfDuplicates );
	
	// static void QuickCreateAsset(UClass* AssetClass, FString AssetName, FString PackagePath);

	/**
	 * Adds prefixes and suffixes to the selected assets based on a TMap of UClasses and their respective prefixes/suffixes.
	 */
	UFUNCTION(CallInEditor, Category = "QuickAssetAction")
	void AddPrefixes();

	/**
	 * Checks selected assets that aren't being used in the project and prompts the user to delete them.
	 */
	UFUNCTION(CallInEditor, Category = "QuickAssetAction")
	void RemoveUnusedAssets();

private:
	// mapping of asset class to the asset prefix
	TMap<UClass*, FString> PrefixMap =
		{
			{UAnimBlueprint::StaticClass(), "ABP_"},
			{UAnimSequence::StaticClass(), TEXT("AM_")},
			{UBlueprint::StaticClass(), TEXT("BP_")},
			{UBlendSpace::StaticClass(), TEXT("BS_")},
			{UCameraShakeBase::StaticClass(), TEXT("CamShake_")},
			{UCurveFloat::StaticClass(), TEXT("CF_")},
			{UCurveLinearColor::StaticClass(), TEXT("CLC_")},
			{UCurveLinearColorAtlas::StaticClass(), TEXT("CLA_")},
			{UCurveTable::StaticClass(), TEXT("CT_")},
			{UEnum::StaticClass(), TEXT("E_")},
			{UFont::StaticClass(), TEXT("Font_")},
			{UDataTable::StaticClass(), TEXT("DT_")},
			{UMaterial::StaticClass(), TEXT("M_")},
			{UMaterialInstance::StaticClass(), TEXT("MI_")},
			{UMaterialInstanceConstant::StaticClass(), TEXT("MI_")},
			{UMaterialFunction::StaticClass(), TEXT("MF_")},
			{UMaterialParameterCollection::StaticClass(), TEXT("MPC_")},
			{UMorphTarget::StaticClass(), TEXT("MT_")},
			{UNiagaraSystem::StaticClass(), TEXT("NS_")},
			{UNiagaraEmitter::StaticClass(), TEXT("NE_")},
			{UParticleSystem::StaticClass(), TEXT("P_")},
			{UParticleEmitter::StaticClass(), TEXT("PE_")},
			{UPhysicsAsset::StaticClass(), TEXT("_PhysicsAsset")},
			{USoundWave::StaticClass(), TEXT("SW_")},
			{USoundCue::StaticClass(), TEXT("_Cue")},
			{USoundAttenuation::StaticClass(), TEXT("_Att")},
			{USoundBase::StaticClass(), TEXT("S_")},
			{UStaticMesh::StaticClass(), TEXT("SM_")},
			{USkeletalMesh::StaticClass(), TEXT("SK_")},
			{USkeleton::StaticClass(), TEXT("_Skeleton")},
			{UTexture2D::StaticClass(), TEXT("T_")},
			{UTextureRenderTarget2D::StaticClass(), TEXT("RT_")},
			{UTextureCube::StaticClass(), TEXT("TC_")},
			{UUserWidget::StaticClass(), TEXT("WBP_")},
		};
};
