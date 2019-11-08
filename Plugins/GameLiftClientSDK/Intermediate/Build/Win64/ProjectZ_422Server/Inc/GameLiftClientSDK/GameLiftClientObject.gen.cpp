// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "GameLiftClientSDK/Public/GameLiftClientObject.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeGameLiftClientObject() {}
// Cross Module References
	GAMELIFTCLIENTSDK_API UClass* Z_Construct_UClass_UGameLiftClientObject_NoRegister();
	GAMELIFTCLIENTSDK_API UClass* Z_Construct_UClass_UGameLiftClientObject();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
	UPackage* Z_Construct_UPackage__Script_GameLiftClientSDK();
	GAMELIFTCLIENTSDK_API UFunction* Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject();
	GAMELIFTCLIENTSDK_API UFunction* Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession();
	GAMELIFTCLIENTSDK_API UClass* Z_Construct_UClass_UGameLiftCreateGameSession_NoRegister();
	GAMELIFTCLIENTSDK_API UScriptStruct* Z_Construct_UScriptStruct_FGameLiftGameSessionConfig();
	GAMELIFTCLIENTSDK_API UFunction* Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession();
	GAMELIFTCLIENTSDK_API UClass* Z_Construct_UClass_UGameLiftCreatePlayerSession_NoRegister();
	GAMELIFTCLIENTSDK_API UFunction* Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession();
	GAMELIFTCLIENTSDK_API UClass* Z_Construct_UClass_UGameLiftDescribeGameSession_NoRegister();
	GAMELIFTCLIENTSDK_API UFunction* Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement();
	GAMELIFTCLIENTSDK_API UClass* Z_Construct_UClass_UGameLiftDescribeGameSessionPlacement_NoRegister();
	GAMELIFTCLIENTSDK_API UFunction* Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues();
	GAMELIFTCLIENTSDK_API UClass* Z_Construct_UClass_UGameLiftDescribeGameSessionQueues_NoRegister();
	GAMELIFTCLIENTSDK_API UFunction* Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions();
	GAMELIFTCLIENTSDK_API UClass* Z_Construct_UClass_UGameLiftSearchGameSessions_NoRegister();
	GAMELIFTCLIENTSDK_API UFunction* Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement();
	GAMELIFTCLIENTSDK_API UClass* Z_Construct_UClass_UGameLiftStartGameSessionPlacement_NoRegister();
// End Cross Module References
	void UGameLiftClientObject::StaticRegisterNativesUGameLiftClientObject()
	{
		UClass* Class = UGameLiftClientObject::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "CreateGameLiftObject", &UGameLiftClientObject::execCreateGameLiftObject },
			{ "CreateGameSession", &UGameLiftClientObject::execCreateGameSession },
			{ "CreatePlayerSession", &UGameLiftClientObject::execCreatePlayerSession },
			{ "DescribeGameSession", &UGameLiftClientObject::execDescribeGameSession },
			{ "DescribeGameSessionPlacement", &UGameLiftClientObject::execDescribeGameSessionPlacement },
			{ "DescribeGameSessionQueues", &UGameLiftClientObject::execDescribeGameSessionQueues },
			{ "SearchGameSessions", &UGameLiftClientObject::execSearchGameSessions },
			{ "StartGameSessionPlacement", &UGameLiftClientObject::execStartGameSessionPlacement },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics
	{
		struct GameLiftClientObject_eventCreateGameLiftObject_Parms
		{
			FString AccessKey;
			FString Secret;
			FString Region;
			bool bUsingGameLiftLocal;
			int32 LocalPort;
			UGameLiftClientObject* ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_LocalPort;
		static void NewProp_bUsingGameLiftLocal_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_bUsingGameLiftLocal;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Region_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Region;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_Secret_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_Secret;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_AccessKey_MetaData[];
#endif
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_AccessKey;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventCreateGameLiftObject_Parms, ReturnValue), Z_Construct_UClass_UGameLiftClientObject_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_LocalPort = { "LocalPort", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventCreateGameLiftObject_Parms, LocalPort), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_bUsingGameLiftLocal_SetBit(void* Obj)
	{
		((GameLiftClientObject_eventCreateGameLiftObject_Parms*)Obj)->bUsingGameLiftLocal = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_bUsingGameLiftLocal = { "bUsingGameLiftLocal", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(GameLiftClientObject_eventCreateGameLiftObject_Parms), &Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_bUsingGameLiftLocal_SetBit, METADATA_PARAMS(nullptr, 0) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_Region_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_Region = { "Region", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventCreateGameLiftObject_Parms, Region), METADATA_PARAMS(Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_Region_MetaData, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_Region_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_Secret_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_Secret = { "Secret", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventCreateGameLiftObject_Parms, Secret), METADATA_PARAMS(Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_Secret_MetaData, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_Secret_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_AccessKey_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_AccessKey = { "AccessKey", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventCreateGameLiftObject_Parms, AccessKey), METADATA_PARAMS(Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_AccessKey_MetaData, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_AccessKey_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_LocalPort,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_bUsingGameLiftLocal,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_Region,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_Secret,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::NewProp_AccessKey,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::Function_MetaDataParams[] = {
		{ "Category", "GameLift Client Object" },
		{ "CPP_Default_bUsingGameLiftLocal", "false" },
		{ "CPP_Default_LocalPort", "9080" },
		{ "CPP_Default_Region", "us-east-1" },
		{ "ModuleRelativePath", "Public/GameLiftClientObject.h" },
		{ "ToolTip", "public static UGameLiftClientObject::CreateGameLiftObject\nCreates a GameLiftClientObject. This function must be called first before accessing any GameLift client functions.\n@param AccessKey [const FString&] AccessKey of your AWS user. @See http://docs.aws.amazon.com/general/latest/gr/managing-aws-access-keys.html\n@param Secret [const FString&] SecretKey of your AWS user. @See http://docs.aws.amazon.com/general/latest/gr/managing-aws-access-keys.html\n@param Region [const FString&] Default is set to us-east-1 (North Virginia).\n@param bUsingGameLiftLocal [bool] If true, then it is assumed you are using GameLift Local. @See http://docs.aws.amazon.com/gamelift/latest/developerguide/integration-testing-local.html#integration-testing-local-server\n@param LocalPort [int32] port to use if bUsingGameLiftLocal is true.\n@return [UGameLiftClientObject*] Returns UGameLiftClientObject*. Use this to create game sessions, player sessions etc." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGameLiftClientObject, nullptr, "CreateGameLiftObject", sizeof(GameLiftClientObject_eventCreateGameLiftObject_Parms), Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics
	{
		struct GameLiftClientObject_eventCreateGameSession_Parms
		{
			FGameLiftGameSessionConfig GameSessionProperties;
			UGameLiftCreateGameSession* ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_GameSessionProperties;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventCreateGameSession_Parms, ReturnValue), Z_Construct_UClass_UGameLiftCreateGameSession_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::NewProp_GameSessionProperties = { "GameSessionProperties", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventCreateGameSession_Parms, GameSessionProperties), Z_Construct_UScriptStruct_FGameLiftGameSessionConfig, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::NewProp_GameSessionProperties,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::Function_MetaDataParams[] = {
		{ "Category", "GameLift Client Object" },
		{ "ModuleRelativePath", "Public/GameLiftClientObject.h" },
		{ "ToolTip", "public UGameLiftClientObject::CreateGameSession\nCreates a multi player game session for players.\nThis action creates a game session record and assigns an available server process in the specified fleet to host the game session.\nA fleet must have an ACTIVE status before a game session can be created in it.\nFrom the return value first bind both success and fail events and then call Activate to create game session.\n@param GameSessionProperties [FGameLiftGameSessionConfig] Settings for the game session you want to create.\n@return [UGameLiftCreateGameSession*] Returns UGameLiftCreateGameSession* Object." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGameLiftClientObject, nullptr, "CreateGameSession", sizeof(GameLiftClientObject_eventCreateGameSession_Parms), Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics
	{
		struct GameLiftClientObject_eventCreatePlayerSession_Parms
		{
			FString GameSessionID;
			FString UniquePlayerID;
			UGameLiftCreatePlayerSession* ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_UniquePlayerID;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_GameSessionID;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventCreatePlayerSession_Parms, ReturnValue), Z_Construct_UClass_UGameLiftCreatePlayerSession_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::NewProp_UniquePlayerID = { "UniquePlayerID", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventCreatePlayerSession_Parms, UniquePlayerID), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::NewProp_GameSessionID = { "GameSessionID", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventCreatePlayerSession_Parms, GameSessionID), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::NewProp_UniquePlayerID,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::NewProp_GameSessionID,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::Function_MetaDataParams[] = {
		{ "Category", "GameLift Client Object" },
		{ "ModuleRelativePath", "Public/GameLiftClientObject.h" },
		{ "ToolTip", "public UGameLiftClientObject::CreatePlayerSession\nAdds a player to a game session and creates a player session record.\nBefore a player can be added, a game session must have an ACTIVE status, have a creation policy of ALLOW_ALL, and have an open player slot.\nFrom the return value first bind both success and fail events and then call Activate to create player session.\n@param GameSessionID [FString] Unique identifier for the game session to add a player to.\n@param UniquePlayerID [FString] Unique identifier for a player. Player IDs are developer-defined.\n@return [UGameLiftCreatePlayerSession*] Returns UGameLiftCreatePlayerSession* Object." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGameLiftClientObject, nullptr, "CreatePlayerSession", sizeof(GameLiftClientObject_eventCreatePlayerSession_Parms), Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics
	{
		struct GameLiftClientObject_eventDescribeGameSession_Parms
		{
			FString GameSessionID;
			UGameLiftDescribeGameSession* ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_GameSessionID;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventDescribeGameSession_Parms, ReturnValue), Z_Construct_UClass_UGameLiftDescribeGameSession_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::NewProp_GameSessionID = { "GameSessionID", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventDescribeGameSession_Parms, GameSessionID), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::NewProp_GameSessionID,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::Function_MetaDataParams[] = {
		{ "Category", "GameLift Client Object" },
		{ "ModuleRelativePath", "Public/GameLiftClientObject.h" },
		{ "ToolTip", "public UGameLiftClientObject::DescribeGameSession\nRetrieves the given game session.\nFrom the return value first bind both success and fail events and then call Activate to describe game session.\n@param GameSessionID [FString] Game Session ID. This can be obtained after a successful create game session.\n@return [UGameLiftDescribeGameSession*] Returns UGameLiftDescribeGameSession* Object." },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGameLiftClientObject, nullptr, "DescribeGameSession", sizeof(GameLiftClientObject_eventDescribeGameSession_Parms), Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics
	{
		struct GameLiftClientObject_eventDescribeGameSessionPlacement_Parms
		{
			FString GameSessionPlacementId;
			UGameLiftDescribeGameSessionPlacement* ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_GameSessionPlacementId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventDescribeGameSessionPlacement_Parms, ReturnValue), Z_Construct_UClass_UGameLiftDescribeGameSessionPlacement_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::NewProp_GameSessionPlacementId = { "GameSessionPlacementId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventDescribeGameSessionPlacement_Parms, GameSessionPlacementId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::NewProp_GameSessionPlacementId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::Function_MetaDataParams[] = {
		{ "Category", "GameLift Client Object" },
		{ "ModuleRelativePath", "Public/GameLiftClientObject.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGameLiftClientObject, nullptr, "DescribeGameSessionPlacement", sizeof(GameLiftClientObject_eventDescribeGameSessionPlacement_Parms), Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics
	{
		struct GameLiftClientObject_eventDescribeGameSessionQueues_Parms
		{
			FString QueueName;
			UGameLiftDescribeGameSessionQueues* ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_QueueName;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventDescribeGameSessionQueues_Parms, ReturnValue), Z_Construct_UClass_UGameLiftDescribeGameSessionQueues_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::NewProp_QueueName = { "QueueName", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventDescribeGameSessionQueues_Parms, QueueName), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::NewProp_QueueName,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::Function_MetaDataParams[] = {
		{ "Category", "GameLift Client Object" },
		{ "ModuleRelativePath", "Public/GameLiftClientObject.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGameLiftClientObject, nullptr, "DescribeGameSessionQueues", sizeof(GameLiftClientObject_eventDescribeGameSessionQueues_Parms), Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics
	{
		struct GameLiftClientObject_eventSearchGameSessions_Parms
		{
			FString FleetId;
			FString AliasId;
			FString FilterExpression;
			FString SortExpression;
			UGameLiftSearchGameSessions* ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_SortExpression;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_FilterExpression;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_AliasId;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_FleetId;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventSearchGameSessions_Parms, ReturnValue), Z_Construct_UClass_UGameLiftSearchGameSessions_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::NewProp_SortExpression = { "SortExpression", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventSearchGameSessions_Parms, SortExpression), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::NewProp_FilterExpression = { "FilterExpression", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventSearchGameSessions_Parms, FilterExpression), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::NewProp_AliasId = { "AliasId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventSearchGameSessions_Parms, AliasId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::NewProp_FleetId = { "FleetId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventSearchGameSessions_Parms, FleetId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::NewProp_SortExpression,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::NewProp_FilterExpression,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::NewProp_AliasId,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::NewProp_FleetId,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::Function_MetaDataParams[] = {
		{ "Category", "GameLift Client Object" },
		{ "ModuleRelativePath", "Public/GameLiftClientObject.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGameLiftClientObject, nullptr, "SearchGameSessions", sizeof(GameLiftClientObject_eventSearchGameSessions_Parms), Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics
	{
		struct GameLiftClientObject_eventStartGameSessionPlacement_Parms
		{
			FString QueueName;
			int32 MaxPlayerCount;
			FString PlacementId;
			UGameLiftStartGameSessionPlacement* ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_PlacementId;
		static const UE4CodeGen_Private::FUnsizedIntPropertyParams NewProp_MaxPlayerCount;
		static const UE4CodeGen_Private::FStrPropertyParams NewProp_QueueName;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventStartGameSessionPlacement_Parms, ReturnValue), Z_Construct_UClass_UGameLiftStartGameSessionPlacement_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::NewProp_PlacementId = { "PlacementId", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventStartGameSessionPlacement_Parms, PlacementId), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FUnsizedIntPropertyParams Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::NewProp_MaxPlayerCount = { "MaxPlayerCount", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventStartGameSessionPlacement_Parms, MaxPlayerCount), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStrPropertyParams Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::NewProp_QueueName = { "QueueName", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Str, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(GameLiftClientObject_eventStartGameSessionPlacement_Parms, QueueName), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::NewProp_ReturnValue,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::NewProp_PlacementId,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::NewProp_MaxPlayerCount,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::NewProp_QueueName,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::Function_MetaDataParams[] = {
		{ "Category", "GameLift Client Object" },
		{ "ModuleRelativePath", "Public/GameLiftClientObject.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_UGameLiftClientObject, nullptr, "StartGameSessionPlacement", sizeof(GameLiftClientObject_eventStartGameSessionPlacement_Parms), Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::PropPointers, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04020401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::Function_MetaDataParams, ARRAY_COUNT(Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_UGameLiftClientObject_NoRegister()
	{
		return UGameLiftClientObject::StaticClass();
	}
	struct Z_Construct_UClass_UGameLiftClientObject_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_UGameLiftClientObject_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_GameLiftClientSDK,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_UGameLiftClientObject_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_UGameLiftClientObject_CreateGameLiftObject, "CreateGameLiftObject" }, // 2085865047
		{ &Z_Construct_UFunction_UGameLiftClientObject_CreateGameSession, "CreateGameSession" }, // 2495659728
		{ &Z_Construct_UFunction_UGameLiftClientObject_CreatePlayerSession, "CreatePlayerSession" }, // 97416196
		{ &Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSession, "DescribeGameSession" }, // 4063305265
		{ &Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionPlacement, "DescribeGameSessionPlacement" }, // 3470784357
		{ &Z_Construct_UFunction_UGameLiftClientObject_DescribeGameSessionQueues, "DescribeGameSessionQueues" }, // 76019964
		{ &Z_Construct_UFunction_UGameLiftClientObject_SearchGameSessions, "SearchGameSessions" }, // 4185635314
		{ &Z_Construct_UFunction_UGameLiftClientObject_StartGameSessionPlacement, "StartGameSessionPlacement" }, // 288144075
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_UGameLiftClientObject_Statics::Class_MetaDataParams[] = {
		{ "BlueprintType", "true" },
		{ "IncludePath", "GameLiftClientObject.h" },
		{ "ModuleRelativePath", "Public/GameLiftClientObject.h" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_UGameLiftClientObject_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<UGameLiftClientObject>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_UGameLiftClientObject_Statics::ClassParams = {
		&UGameLiftClientObject::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		ARRAY_COUNT(DependentSingletons),
		ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x001000A0u,
		METADATA_PARAMS(Z_Construct_UClass_UGameLiftClientObject_Statics::Class_MetaDataParams, ARRAY_COUNT(Z_Construct_UClass_UGameLiftClientObject_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_UGameLiftClientObject()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_UGameLiftClientObject_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(UGameLiftClientObject, 1630781855);
	template<> GAMELIFTCLIENTSDK_API UClass* StaticClass<UGameLiftClientObject>()
	{
		return UGameLiftClientObject::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_UGameLiftClientObject(Z_Construct_UClass_UGameLiftClientObject, &UGameLiftClientObject::StaticClass, TEXT("/Script/GameLiftClientSDK"), TEXT("UGameLiftClientObject"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(UGameLiftClientObject);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
