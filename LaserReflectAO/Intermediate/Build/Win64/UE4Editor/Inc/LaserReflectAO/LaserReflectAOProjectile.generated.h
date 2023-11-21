// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class UPrimitiveComponent;
class AActor;
struct FVector;
struct FHitResult;
#ifdef LASERREFLECTAO_LaserReflectAOProjectile_generated_h
#error "LaserReflectAOProjectile.generated.h already included, missing '#pragma once' in LaserReflectAOProjectile.h"
#endif
#define LASERREFLECTAO_LaserReflectAOProjectile_generated_h

#define LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_SPARSE_DATA
#define LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execOnHit);


#define LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execOnHit);


#define LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesALaserReflectAOProjectile(); \
	friend struct Z_Construct_UClass_ALaserReflectAOProjectile_Statics; \
public: \
	DECLARE_CLASS(ALaserReflectAOProjectile, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/LaserReflectAO"), NO_API) \
	DECLARE_SERIALIZER(ALaserReflectAOProjectile) \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_INCLASS \
private: \
	static void StaticRegisterNativesALaserReflectAOProjectile(); \
	friend struct Z_Construct_UClass_ALaserReflectAOProjectile_Statics; \
public: \
	DECLARE_CLASS(ALaserReflectAOProjectile, AActor, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/LaserReflectAO"), NO_API) \
	DECLARE_SERIALIZER(ALaserReflectAOProjectile) \
	static const TCHAR* StaticConfigName() {return TEXT("Game");} \



#define LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API ALaserReflectAOProjectile(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(ALaserReflectAOProjectile) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ALaserReflectAOProjectile); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ALaserReflectAOProjectile); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ALaserReflectAOProjectile(ALaserReflectAOProjectile&&); \
	NO_API ALaserReflectAOProjectile(const ALaserReflectAOProjectile&); \
public:


#define LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API ALaserReflectAOProjectile(ALaserReflectAOProjectile&&); \
	NO_API ALaserReflectAOProjectile(const ALaserReflectAOProjectile&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, ALaserReflectAOProjectile); \
	DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(ALaserReflectAOProjectile); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(ALaserReflectAOProjectile)


#define LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__CollisionComp() { return STRUCT_OFFSET(ALaserReflectAOProjectile, CollisionComp); } \
	FORCEINLINE static uint32 __PPO__ProjectileMovement() { return STRUCT_OFFSET(ALaserReflectAOProjectile, ProjectileMovement); }


#define LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_12_PROLOG
#define LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_PRIVATE_PROPERTY_OFFSET \
	LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_SPARSE_DATA \
	LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_RPC_WRAPPERS \
	LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_INCLASS \
	LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_PRIVATE_PROPERTY_OFFSET \
	LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_SPARSE_DATA \
	LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_RPC_WRAPPERS_NO_PURE_DECLS \
	LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_INCLASS_NO_PURE_DECLS \
	LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h_15_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> LASERREFLECTAO_API UClass* StaticClass<class ALaserReflectAOProjectile>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID LaserReflectAO_Source_LaserReflectAO_LaserReflectAOProjectile_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
