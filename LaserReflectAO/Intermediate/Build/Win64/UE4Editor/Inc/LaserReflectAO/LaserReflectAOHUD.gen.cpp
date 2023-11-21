// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "LaserReflectAO/LaserReflectAOHUD.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeLaserReflectAOHUD() {}
// Cross Module References
	LASERREFLECTAO_API UClass* Z_Construct_UClass_ALaserReflectAOHUD_NoRegister();
	LASERREFLECTAO_API UClass* Z_Construct_UClass_ALaserReflectAOHUD();
	ENGINE_API UClass* Z_Construct_UClass_AHUD();
	UPackage* Z_Construct_UPackage__Script_LaserReflectAO();
// End Cross Module References
	void ALaserReflectAOHUD::StaticRegisterNativesALaserReflectAOHUD()
	{
	}
	UClass* Z_Construct_UClass_ALaserReflectAOHUD_NoRegister()
	{
		return ALaserReflectAOHUD::StaticClass();
	}
	struct Z_Construct_UClass_ALaserReflectAOHUD_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ALaserReflectAOHUD_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AHUD,
		(UObject* (*)())Z_Construct_UPackage__Script_LaserReflectAO,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ALaserReflectAOHUD_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Rendering Actor Input Replication" },
		{ "IncludePath", "LaserReflectAOHUD.h" },
		{ "ModuleRelativePath", "LaserReflectAOHUD.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ALaserReflectAOHUD_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ALaserReflectAOHUD>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ALaserReflectAOHUD_Statics::ClassParams = {
		&ALaserReflectAOHUD::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008002ACu,
		METADATA_PARAMS(Z_Construct_UClass_ALaserReflectAOHUD_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ALaserReflectAOHUD_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ALaserReflectAOHUD()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ALaserReflectAOHUD_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ALaserReflectAOHUD, 931771181);
	template<> LASERREFLECTAO_API UClass* StaticClass<ALaserReflectAOHUD>()
	{
		return ALaserReflectAOHUD::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ALaserReflectAOHUD(Z_Construct_UClass_ALaserReflectAOHUD, &ALaserReflectAOHUD::StaticClass, TEXT("/Script/LaserReflectAO"), TEXT("ALaserReflectAOHUD"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ALaserReflectAOHUD);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
