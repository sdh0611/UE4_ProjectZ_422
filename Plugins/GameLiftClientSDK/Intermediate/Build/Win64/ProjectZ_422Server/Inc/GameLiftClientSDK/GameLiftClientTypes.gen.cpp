// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameLiftClientSDK/Public/GameLiftClientTypes.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGameLiftClientTypes() {}
// Cross Module References
	GAMELIFTCLIENTSDK_API UEnum* Z_Construct_UEnum_GameLiftClientSDK_EGameLiftGameSessionStatus();
	UPackage* Z_Construct_UPackage__Script_GameLiftClientSDK();
	GAMELIFTCLIENTSDK_API UScriptStruct* Z_Construct_UScriptStruct_FGameLiftGameSessionConfig();
	GAMELIFTCLIENTSDK_API UScriptStruct* Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties();
// End Cross Module References
	static UEnum* EGameLiftGameSessionStatus_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_GameLiftClientSDK_EGameLiftGameSessionStatus, Z_Construct_UPackage__Script_GameLiftClientSDK(), TEXT("EGameLiftGameSessionStatus"));
		}
		return Singleton;
	}
	template<> GAMELIFTCLIENTSDK_API UEnum* StaticEnum<EGameLiftGameSessionStatus>()
	{
		return EGameLiftGameSessionStatus_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_EGameLiftGameSessionStatus(EGameLiftGameSessionStatus_StaticEnum, TEXT("/Script/GameLiftClientSDK"), TEXT("EGameLiftGameSessionStatus"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_GameLiftClientSDK_EGameLiftGameSessionStatus_Hash() { return 3091959603U; }
	UEnum* Z_Construct_UEnum_GameLiftClientSDK_EGameLiftGameSessionStatus()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_GameLiftClientSDK();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("EGameLiftGameSessionStatus"), 0, Get_Z_Construct_UEnum_GameLiftClientSDK_EGameLiftGameSessionStatus_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "EGameLiftGameSessionStatus::STATUS_Activating", (int64)EGameLiftGameSessionStatus::STATUS_Activating },
				{ "EGameLiftGameSessionStatus::STATUS_Active", (int64)EGameLiftGameSessionStatus::STATUS_Active },
				{ "EGameLiftGameSessionStatus::STATUS_Error", (int64)EGameLiftGameSessionStatus::STATUS_Error },
				{ "EGameLiftGameSessionStatus::STATUS_NotSet", (int64)EGameLiftGameSessionStatus::STATUS_NotSet },
				{ "EGameLiftGameSessionStatus::STATUS_Terminating", (int64)EGameLiftGameSessionStatus::STATUS_Terminating },
				{ "EGameLiftGameSessionStatus::STATUS_Terminated", (int64)EGameLiftGameSessionStatus::STATUS_Terminated },
				{ "EGameLiftGameSessionStatus::STATUS_NoStatus", (int64)EGameLiftGameSessionStatus::STATUS_NoStatus },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "ModuleRelativePath", "Public/GameLiftClientTypes.h" },
				{ "STATUS_Activating.DisplayName", "Activating" },
				{ "STATUS_Active.DisplayName", "Active" },
				{ "STATUS_Error.DisplayName", "Error" },
				{ "STATUS_NoStatus.Hidden", "" },
				{ "STATUS_NotSet.DisplayName", "Not Set" },
				{ "STATUS_Terminated.DisplayName", "Terminated" },
				{ "STATUS_Terminating.DisplayName", "Terminating" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_GameLiftClientSDK,
				nullptr,
				"EGameLiftGameSessionStatus",
				"EGameLiftGameSessionStatus",
				Enumerators,
				ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
class UScriptStruct* FGameLiftGameSessionConfig::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern GAMELIFTCLIENTSDK_API uint32 Get_Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FGameLiftGameSessionConfig, Z_Construct_UPackage__Script_GameLiftClientSDK(), TEXT("GameLiftGameSessionConfig"), sizeof(FGameLiftGameSessionConfig), Get_Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Hash());
	}
	return Singleton;
}
template<> GAMELIFTCLIENTSDK_API UScriptStruct* StaticStruct<FGameLiftGameSessionConfig>()
{
	return FGameLiftGameSessionConfig::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FGameLiftGameSessionConfig(FGameLiftGameSessionConfig::StaticStruct, TEXT("/Script/GameLiftClientSDK"), TEXT("GameLiftGameSessionConfig"), false, nullptr, nullptr);
static struct FScriptStruct_GameLiftClientSDK_StaticRegisterNativesFGameLiftGameSessionConfig
{
	FScriptStruct_GameLiftClientSDK_StaticRegisterNativesFGameLiftGameSessionConfig()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("GameLiftGameSessionConfig")),new UScriptStruct::TCppStructOps<FGameLiftGameSessionConfig>);
	}
} ScriptStruct_GameLiftClientSDK_StaticRegisterNativesFGameLiftGameSessionConfig;
	struct Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_GameSessionProperties_MetaData[];
#endif
		static const UE4CodeGen_Private::FArrayPropertyParams NewProp_GameSessionProperties;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_GameSessionProperties_Inner;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AliasID_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_AliasID;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_MaxPlayers_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_MaxPlayers;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "ModuleRelativePath", "Public/GameLiftClientTypes.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FGameLiftGameSessionConfig>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_GameSessionProperties_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "GameLiftGameSessionConfig" },
		{ "ModuleRelativePath", "Public/GameLiftClientTypes.h" },
		{ "ToolTip", "Set of developer-defined properties for a game session, formatted as a set of type:value pairs.\n       * These properties are included in the GameSession object, which is passed to the game server with a request to start a new game session" },
	};
#endif
	const UE4CodeGen_Private::FArrayPropertyParams Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_GameSessionProperties = { "GameSessionProperties", nullptr, (EPropertyFlags)0x0040000000000005, UE4CodeGen_Private::EPropertyGenFlags::Array, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FGameLiftGameSessionConfig, GameSessionProperties), METADATA_PARAMS(Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_GameSessionProperties_MetaData, ARRAY_COUNT(Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_GameSessionProperties_MetaData)) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_GameSessionProperties_Inner = { "GameSessionProperties", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_AliasID_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "GameLiftGameSessionConfig" },
		{ "ModuleRelativePath", "Public/GameLiftClientTypes.h" },
		{ "ToolTip", "* Unique identifier for an alias associated with the fleet to create a game session in.\n* You can switch gameplay and players to a new fleet without changing your game client or other game components.\n* For example, for games in production, using an alias allows you to seamlessly redirect your player base to a new game server update.\n* @See http://docs.aws.amazon.com/gamelift/latest/developerguide/aliases-creating.html\n* @See http://docs.aws.amazon.com/gamelift/latest/apireference/API_CreateAlias.html" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_AliasID = { "AliasID", nullptr, (EPropertyFlags)0x0040000000000005, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FGameLiftGameSessionConfig, AliasID), METADATA_PARAMS(Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_AliasID_MetaData, ARRAY_COUNT(Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_AliasID_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_MaxPlayers_MetaData[] = {
		{ "AllowPrivateAccess", "true" },
		{ "Category", "GameLiftGameSessionConfig" },
		{ "ModuleRelativePath", "Public/GameLiftClientTypes.h" },
		{ "ToolTip", "Maximum number of players that can be connected simultaneously to the game session." },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_MaxPlayers = { "MaxPlayers", nullptr, (EPropertyFlags)0x0040000000000005, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FGameLiftGameSessionConfig, MaxPlayers), METADATA_PARAMS(Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_MaxPlayers_MetaData, ARRAY_COUNT(Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_MaxPlayers_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_GameSessionProperties,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_GameSessionProperties_Inner,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_AliasID,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::NewProp_MaxPlayers,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_GameLiftClientSDK,
		nullptr,
		&NewStructOps,
		"GameLiftGameSessionConfig",
		sizeof(FGameLiftGameSessionConfig),
		alignof(FGameLiftGameSessionConfig),
		Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::PropPointers,
		ARRAY_COUNT(Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::Struct_MetaDataParams, ARRAY_COUNT(Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FGameLiftGameSessionConfig()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_GameLiftClientSDK();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("GameLiftGameSessionConfig"), sizeof(FGameLiftGameSessionConfig), Get_Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FGameLiftGameSessionConfig_Hash() { return 806108464U; }
class UScriptStruct* FGameLiftGameSessionServerProperties::StaticStruct()
{
	static class UScriptStruct* Singleton = NULL;
	if (!Singleton)
	{
		extern GAMELIFTCLIENTSDK_API uint32 Get_Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Hash();
		Singleton = GetStaticStruct(Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties, Z_Construct_UPackage__Script_GameLiftClientSDK(), TEXT("GameLiftGameSessionServerProperties"), sizeof(FGameLiftGameSessionServerProperties), Get_Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Hash());
	}
	return Singleton;
}
template<> GAMELIFTCLIENTSDK_API UScriptStruct* StaticStruct<FGameLiftGameSessionServerProperties>()
{
	return FGameLiftGameSessionServerProperties::StaticStruct();
}
static FCompiledInDeferStruct Z_CompiledInDeferStruct_UScriptStruct_FGameLiftGameSessionServerProperties(FGameLiftGameSessionServerProperties::StaticStruct, TEXT("/Script/GameLiftClientSDK"), TEXT("GameLiftGameSessionServerProperties"), false, nullptr, nullptr);
static struct FScriptStruct_GameLiftClientSDK_StaticRegisterNativesFGameLiftGameSessionServerProperties
{
	FScriptStruct_GameLiftClientSDK_StaticRegisterNativesFGameLiftGameSessionServerProperties()
	{
		UScriptStruct::DeferCppStructOps(FName(TEXT("GameLiftGameSessionServerProperties")),new UScriptStruct::TCppStructOps<FGameLiftGameSessionServerProperties>);
	}
} ScriptStruct_GameLiftClientSDK_StaticRegisterNativesFGameLiftGameSessionServerProperties;
	struct Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics
	{
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Struct_MetaDataParams[];
#endif
		static void* NewStructOps();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Value_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Value;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Key_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Key;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const UE4CodeGen_Private::FStructParams ReturnStructParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::Struct_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IsBlueprintBase", "true" },
		{ "ModuleRelativePath", "Public/GameLiftClientTypes.h" },
	};
#endif
	void* Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::NewStructOps()
	{
		return (UScriptStruct::ICppStructOps*)new UScriptStruct::TCppStructOps<FGameLiftGameSessionServerProperties>();
	}
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::NewProp_Value_MetaData[] = {
		{ "Category", "GameLiftGameSessionServerProperties" },
		{ "ModuleRelativePath", "Public/GameLiftClientTypes.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::NewProp_Value = { "Value", nullptr, (EPropertyFlags)0x0010000000000004, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FGameLiftGameSessionServerProperties, Value), METADATA_PARAMS(Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::NewProp_Value_MetaData, ARRAY_COUNT(Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::NewProp_Value_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::NewProp_Key_MetaData[] = {
		{ "Category", "GameLiftGameSessionServerProperties" },
		{ "ModuleRelativePath", "Public/GameLiftClientTypes.h" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::NewProp_Key = { "Key", nullptr, (EPropertyFlags)0x0010000000000004, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(FGameLiftGameSessionServerProperties, Key), METADATA_PARAMS(Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::NewProp_Key_MetaData, ARRAY_COUNT(Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::NewProp_Key_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::NewProp_Value,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::NewProp_Key,
	};
	const UE4CodeGen_Private::FStructParams Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::ReturnStructParams = {
		(UObject* (*)())Z_Construct_UPackage__Script_GameLiftClientSDK,
		nullptr,
		&NewStructOps,
		"GameLiftGameSessionServerProperties",
		sizeof(FGameLiftGameSessionServerProperties),
		alignof(FGameLiftGameSessionServerProperties),
		Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::PropPointers,
		ARRAY_COUNT(Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::PropPointers),
		RF_Public|RF_Transient|RF_MarkAsNative,
		EStructFlags(0x00000001),
		METADATA_PARAMS(Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::Struct_MetaDataParams, ARRAY_COUNT(Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::Struct_MetaDataParams))
	};
	UScriptStruct* Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties()
	{
#if WITH_HOT_RELOAD
		extern uint32 Get_Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Hash();
		UPackage* Outer = Z_Construct_UPackage__Script_GameLiftClientSDK();
		static UScriptStruct* ReturnStruct = FindExistingStructIfHotReloadOrDynamic(Outer, TEXT("GameLiftGameSessionServerProperties"), sizeof(FGameLiftGameSessionServerProperties), Get_Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Hash(), false);
#else
		static UScriptStruct* ReturnStruct = nullptr;
#endif
		if (!ReturnStruct)
		{
			UE4CodeGen_Private::ConstructUScriptStruct(ReturnStruct, Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Statics::ReturnStructParams);
		}
		return ReturnStruct;
	}
	uint32 Get_Z_Construct_UScriptStruct_FGameLiftGameSessionServerProperties_Hash() { return 2330464482U; }
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
