// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
enum class EGameLiftGameSessionStatus : uint8;
enum class EActivateStatus : uint8;
#ifdef GAMELIFTCLIENTSDK_GameLiftClientApi_generated_h
#error "GameLiftClientApi.generated.h already included, missing '#pragma once' in GameLiftClientApi.h"
#endif
#define GAMELIFTCLIENTSDK_GameLiftClientApi_generated_h

#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_230_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnDescribeGameSessionPlacementFailed_Parms \
{ \
	FString ErrorMessage; \
}; \
static inline void FOnDescribeGameSessionPlacementFailed_DelegateWrapper(const FMulticastScriptDelegate& OnDescribeGameSessionPlacementFailed, const FString& ErrorMessage) \
{ \
	_Script_GameLiftClientSDK_eventOnDescribeGameSessionPlacementFailed_Parms Parms; \
	Parms.ErrorMessage=ErrorMessage; \
	OnDescribeGameSessionPlacementFailed.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_229_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnDescribeGameSessionPlacementSuccess_Parms \
{ \
	FString GameSessionId; \
	FString PlacementId; \
	int32 Status; \
}; \
static inline void FOnDescribeGameSessionPlacementSuccess_DelegateWrapper(const FMulticastScriptDelegate& OnDescribeGameSessionPlacementSuccess, const FString& GameSessionId, const FString& PlacementId, int32 const& Status) \
{ \
	_Script_GameLiftClientSDK_eventOnDescribeGameSessionPlacementSuccess_Parms Parms; \
	Parms.GameSessionId=GameSessionId; \
	Parms.PlacementId=PlacementId; \
	Parms.Status=Status; \
	OnDescribeGameSessionPlacementSuccess.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_196_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnStartGameSessionPlacementFailed_Parms \
{ \
	FString ErrorMessage; \
}; \
static inline void FOnStartGameSessionPlacementFailed_DelegateWrapper(const FMulticastScriptDelegate& OnStartGameSessionPlacementFailed, const FString& ErrorMessage) \
{ \
	_Script_GameLiftClientSDK_eventOnStartGameSessionPlacementFailed_Parms Parms; \
	Parms.ErrorMessage=ErrorMessage; \
	OnStartGameSessionPlacementFailed.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_195_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnStartGameSessionPlacementSuccess_Parms \
{ \
	FString GameSessionId; \
	FString PlacementId; \
	int32 Status; \
}; \
static inline void FOnStartGameSessionPlacementSuccess_DelegateWrapper(const FMulticastScriptDelegate& OnStartGameSessionPlacementSuccess, const FString& GameSessionId, const FString& PlacementId, int32 const& Status) \
{ \
	_Script_GameLiftClientSDK_eventOnStartGameSessionPlacementSuccess_Parms Parms; \
	Parms.GameSessionId=GameSessionId; \
	Parms.PlacementId=PlacementId; \
	Parms.Status=Status; \
	OnStartGameSessionPlacementSuccess.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_161_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnSearchGameSessionsFailed_Parms \
{ \
	FString ErrorMessage; \
}; \
static inline void FOnSearchGameSessionsFailed_DelegateWrapper(const FMulticastScriptDelegate& OnSearchGameSessionsFailed, const FString& ErrorMessage) \
{ \
	_Script_GameLiftClientSDK_eventOnSearchGameSessionsFailed_Parms Parms; \
	Parms.ErrorMessage=ErrorMessage; \
	OnSearchGameSessionsFailed.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_160_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnSearchGameSessionsSuccess_Parms \
{ \
	TArray<FString> GameSessionIds; \
}; \
static inline void FOnSearchGameSessionsSuccess_DelegateWrapper(const FMulticastScriptDelegate& OnSearchGameSessionsSuccess, TArray<FString> const& GameSessionIds) \
{ \
	_Script_GameLiftClientSDK_eventOnSearchGameSessionsSuccess_Parms Parms; \
	Parms.GameSessionIds=GameSessionIds; \
	OnSearchGameSessionsSuccess.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_129_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnDescribeGameSessionQueuesFailed_Parms \
{ \
	FString ErrorMessage; \
}; \
static inline void FOnDescribeGameSessionQueuesFailed_DelegateWrapper(const FMulticastScriptDelegate& OnDescribeGameSessionQueuesFailed, const FString& ErrorMessage) \
{ \
	_Script_GameLiftClientSDK_eventOnDescribeGameSessionQueuesFailed_Parms Parms; \
	Parms.ErrorMessage=ErrorMessage; \
	OnDescribeGameSessionQueuesFailed.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_128_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnDescribeGameSessionQueuesSuccess_Parms \
{ \
	TArray<FString> FleetARNs; \
}; \
static inline void FOnDescribeGameSessionQueuesSuccess_DelegateWrapper(const FMulticastScriptDelegate& OnDescribeGameSessionQueuesSuccess, TArray<FString> const& FleetARNs) \
{ \
	_Script_GameLiftClientSDK_eventOnDescribeGameSessionQueuesSuccess_Parms Parms; \
	Parms.FleetARNs=FleetARNs; \
	OnDescribeGameSessionQueuesSuccess.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_97_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnCreatePlayerSessionFailed_Parms \
{ \
	FString ErrorMessage; \
}; \
static inline void FOnCreatePlayerSessionFailed_DelegateWrapper(const FMulticastScriptDelegate& OnCreatePlayerSessionFailed, const FString& ErrorMessage) \
{ \
	_Script_GameLiftClientSDK_eventOnCreatePlayerSessionFailed_Parms Parms; \
	Parms.ErrorMessage=ErrorMessage; \
	OnCreatePlayerSessionFailed.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_96_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnCreatePlayerSessionSuccess_Parms \
{ \
	FString IPAddress; \
	FString Port; \
	FString PlayerSessionID; \
	int32 PlayerSessionStatus; \
}; \
static inline void FOnCreatePlayerSessionSuccess_DelegateWrapper(const FMulticastScriptDelegate& OnCreatePlayerSessionSuccess, const FString& IPAddress, const FString& Port, const FString& PlayerSessionID, int32 const& PlayerSessionStatus) \
{ \
	_Script_GameLiftClientSDK_eventOnCreatePlayerSessionSuccess_Parms Parms; \
	Parms.IPAddress=IPAddress; \
	Parms.Port=Port; \
	Parms.PlayerSessionID=PlayerSessionID; \
	Parms.PlayerSessionStatus=PlayerSessionStatus; \
	OnCreatePlayerSessionSuccess.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_65_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnDescribeGameSessionStateFailed_Parms \
{ \
	FString ErrorMessage; \
}; \
static inline void FOnDescribeGameSessionStateFailed_DelegateWrapper(const FMulticastScriptDelegate& OnDescribeGameSessionStateFailed, const FString& ErrorMessage) \
{ \
	_Script_GameLiftClientSDK_eventOnDescribeGameSessionStateFailed_Parms Parms; \
	Parms.ErrorMessage=ErrorMessage; \
	OnDescribeGameSessionStateFailed.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_64_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnDescribeGameSessionStateSuccess_Parms \
{ \
	FString SessionID; \
	EGameLiftGameSessionStatus SessionState; \
}; \
static inline void FOnDescribeGameSessionStateSuccess_DelegateWrapper(const FMulticastScriptDelegate& OnDescribeGameSessionStateSuccess, const FString& SessionID, EGameLiftGameSessionStatus SessionState) \
{ \
	_Script_GameLiftClientSDK_eventOnDescribeGameSessionStateSuccess_Parms Parms; \
	Parms.SessionID=SessionID; \
	Parms.SessionState=SessionState; \
	OnDescribeGameSessionStateSuccess.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_33_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnCreateGameSessionFailed_Parms \
{ \
	FString ErrorMessage; \
}; \
static inline void FOnCreateGameSessionFailed_DelegateWrapper(const FMulticastScriptDelegate& OnCreateGameSessionFailed, const FString& ErrorMessage) \
{ \
	_Script_GameLiftClientSDK_eventOnCreateGameSessionFailed_Parms Parms; \
	Parms.ErrorMessage=ErrorMessage; \
	OnCreateGameSessionFailed.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_32_DELEGATE \
struct _Script_GameLiftClientSDK_eventOnCreateGameSessionSuccess_Parms \
{ \
	FString GameSessionID; \
}; \
static inline void FOnCreateGameSessionSuccess_DelegateWrapper(const FMulticastScriptDelegate& OnCreateGameSessionSuccess, const FString& GameSessionID) \
{ \
	_Script_GameLiftClientSDK_eventOnCreateGameSessionSuccess_Parms Parms; \
	Parms.GameSessionID=GameSessionID; \
	OnCreateGameSessionSuccess.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUGameLiftCreateGameSession(); \
	friend struct Z_Construct_UClass_UGameLiftCreateGameSession_Statics; \
public: \
	DECLARE_CLASS(UGameLiftCreateGameSession, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftCreateGameSession)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_INCLASS \
private: \
	static void StaticRegisterNativesUGameLiftCreateGameSession(); \
	friend struct Z_Construct_UClass_UGameLiftCreateGameSession_Statics; \
public: \
	DECLARE_CLASS(UGameLiftCreateGameSession, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftCreateGameSession)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftCreateGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftCreateGameSession) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftCreateGameSession); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftCreateGameSession); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftCreateGameSession(UGameLiftCreateGameSession&&); \
	NO_API UGameLiftCreateGameSession(const UGameLiftCreateGameSession&); \
public:


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftCreateGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftCreateGameSession(UGameLiftCreateGameSession&&); \
	NO_API UGameLiftCreateGameSession(const UGameLiftCreateGameSession&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftCreateGameSession); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftCreateGameSession); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftCreateGameSession)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_PRIVATE_PROPERTY_OFFSET
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_34_PROLOG
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_RPC_WRAPPERS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_INCLASS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_RPC_WRAPPERS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_INCLASS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_37_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GAMELIFTCLIENTSDK_API UClass* StaticClass<class UGameLiftCreateGameSession>();

#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUGameLiftDescribeGameSession(); \
	friend struct Z_Construct_UClass_UGameLiftDescribeGameSession_Statics; \
public: \
	DECLARE_CLASS(UGameLiftDescribeGameSession, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftDescribeGameSession)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_INCLASS \
private: \
	static void StaticRegisterNativesUGameLiftDescribeGameSession(); \
	friend struct Z_Construct_UClass_UGameLiftDescribeGameSession_Statics; \
public: \
	DECLARE_CLASS(UGameLiftDescribeGameSession, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftDescribeGameSession)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftDescribeGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftDescribeGameSession) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftDescribeGameSession); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftDescribeGameSession); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftDescribeGameSession(UGameLiftDescribeGameSession&&); \
	NO_API UGameLiftDescribeGameSession(const UGameLiftDescribeGameSession&); \
public:


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftDescribeGameSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftDescribeGameSession(UGameLiftDescribeGameSession&&); \
	NO_API UGameLiftDescribeGameSession(const UGameLiftDescribeGameSession&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftDescribeGameSession); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftDescribeGameSession); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftDescribeGameSession)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_PRIVATE_PROPERTY_OFFSET
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_66_PROLOG
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_RPC_WRAPPERS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_INCLASS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_RPC_WRAPPERS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_INCLASS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_69_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GAMELIFTCLIENTSDK_API UClass* StaticClass<class UGameLiftDescribeGameSession>();

#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUGameLiftCreatePlayerSession(); \
	friend struct Z_Construct_UClass_UGameLiftCreatePlayerSession_Statics; \
public: \
	DECLARE_CLASS(UGameLiftCreatePlayerSession, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftCreatePlayerSession)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_INCLASS \
private: \
	static void StaticRegisterNativesUGameLiftCreatePlayerSession(); \
	friend struct Z_Construct_UClass_UGameLiftCreatePlayerSession_Statics; \
public: \
	DECLARE_CLASS(UGameLiftCreatePlayerSession, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftCreatePlayerSession)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftCreatePlayerSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftCreatePlayerSession) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftCreatePlayerSession); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftCreatePlayerSession); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftCreatePlayerSession(UGameLiftCreatePlayerSession&&); \
	NO_API UGameLiftCreatePlayerSession(const UGameLiftCreatePlayerSession&); \
public:


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftCreatePlayerSession(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftCreatePlayerSession(UGameLiftCreatePlayerSession&&); \
	NO_API UGameLiftCreatePlayerSession(const UGameLiftCreatePlayerSession&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftCreatePlayerSession); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftCreatePlayerSession); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftCreatePlayerSession)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_PRIVATE_PROPERTY_OFFSET
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_98_PROLOG
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_RPC_WRAPPERS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_INCLASS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_RPC_WRAPPERS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_INCLASS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_101_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GAMELIFTCLIENTSDK_API UClass* StaticClass<class UGameLiftCreatePlayerSession>();

#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUGameLiftDescribeGameSessionQueues(); \
	friend struct Z_Construct_UClass_UGameLiftDescribeGameSessionQueues_Statics; \
public: \
	DECLARE_CLASS(UGameLiftDescribeGameSessionQueues, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftDescribeGameSessionQueues)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_INCLASS \
private: \
	static void StaticRegisterNativesUGameLiftDescribeGameSessionQueues(); \
	friend struct Z_Construct_UClass_UGameLiftDescribeGameSessionQueues_Statics; \
public: \
	DECLARE_CLASS(UGameLiftDescribeGameSessionQueues, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftDescribeGameSessionQueues)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftDescribeGameSessionQueues(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftDescribeGameSessionQueues) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftDescribeGameSessionQueues); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftDescribeGameSessionQueues); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftDescribeGameSessionQueues(UGameLiftDescribeGameSessionQueues&&); \
	NO_API UGameLiftDescribeGameSessionQueues(const UGameLiftDescribeGameSessionQueues&); \
public:


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftDescribeGameSessionQueues(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftDescribeGameSessionQueues(UGameLiftDescribeGameSessionQueues&&); \
	NO_API UGameLiftDescribeGameSessionQueues(const UGameLiftDescribeGameSessionQueues&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftDescribeGameSessionQueues); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftDescribeGameSessionQueues); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftDescribeGameSessionQueues)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_PRIVATE_PROPERTY_OFFSET
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_130_PROLOG
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_RPC_WRAPPERS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_INCLASS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_RPC_WRAPPERS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_INCLASS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_133_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GAMELIFTCLIENTSDK_API UClass* StaticClass<class UGameLiftDescribeGameSessionQueues>();

#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUGameLiftSearchGameSessions(); \
	friend struct Z_Construct_UClass_UGameLiftSearchGameSessions_Statics; \
public: \
	DECLARE_CLASS(UGameLiftSearchGameSessions, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftSearchGameSessions)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_INCLASS \
private: \
	static void StaticRegisterNativesUGameLiftSearchGameSessions(); \
	friend struct Z_Construct_UClass_UGameLiftSearchGameSessions_Statics; \
public: \
	DECLARE_CLASS(UGameLiftSearchGameSessions, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftSearchGameSessions)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftSearchGameSessions(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftSearchGameSessions) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftSearchGameSessions); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftSearchGameSessions); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftSearchGameSessions(UGameLiftSearchGameSessions&&); \
	NO_API UGameLiftSearchGameSessions(const UGameLiftSearchGameSessions&); \
public:


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftSearchGameSessions(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftSearchGameSessions(UGameLiftSearchGameSessions&&); \
	NO_API UGameLiftSearchGameSessions(const UGameLiftSearchGameSessions&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftSearchGameSessions); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftSearchGameSessions); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftSearchGameSessions)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_PRIVATE_PROPERTY_OFFSET
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_162_PROLOG
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_RPC_WRAPPERS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_INCLASS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_RPC_WRAPPERS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_INCLASS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_165_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GAMELIFTCLIENTSDK_API UClass* StaticClass<class UGameLiftSearchGameSessions>();

#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUGameLiftStartGameSessionPlacement(); \
	friend struct Z_Construct_UClass_UGameLiftStartGameSessionPlacement_Statics; \
public: \
	DECLARE_CLASS(UGameLiftStartGameSessionPlacement, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftStartGameSessionPlacement)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_INCLASS \
private: \
	static void StaticRegisterNativesUGameLiftStartGameSessionPlacement(); \
	friend struct Z_Construct_UClass_UGameLiftStartGameSessionPlacement_Statics; \
public: \
	DECLARE_CLASS(UGameLiftStartGameSessionPlacement, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftStartGameSessionPlacement)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftStartGameSessionPlacement(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftStartGameSessionPlacement) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftStartGameSessionPlacement); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftStartGameSessionPlacement); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftStartGameSessionPlacement(UGameLiftStartGameSessionPlacement&&); \
	NO_API UGameLiftStartGameSessionPlacement(const UGameLiftStartGameSessionPlacement&); \
public:


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftStartGameSessionPlacement(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftStartGameSessionPlacement(UGameLiftStartGameSessionPlacement&&); \
	NO_API UGameLiftStartGameSessionPlacement(const UGameLiftStartGameSessionPlacement&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftStartGameSessionPlacement); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftStartGameSessionPlacement); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftStartGameSessionPlacement)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_PRIVATE_PROPERTY_OFFSET
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_197_PROLOG
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_RPC_WRAPPERS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_INCLASS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_RPC_WRAPPERS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_INCLASS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_200_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GAMELIFTCLIENTSDK_API UClass* StaticClass<class UGameLiftStartGameSessionPlacement>();

#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execActivate) \
	{ \
		P_FINISH; \
		P_NATIVE_BEGIN; \
		*(EActivateStatus*)Z_Param__Result=P_THIS->Activate(); \
		P_NATIVE_END; \
	}


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUGameLiftDescribeGameSessionPlacement(); \
	friend struct Z_Construct_UClass_UGameLiftDescribeGameSessionPlacement_Statics; \
public: \
	DECLARE_CLASS(UGameLiftDescribeGameSessionPlacement, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftDescribeGameSessionPlacement)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_INCLASS \
private: \
	static void StaticRegisterNativesUGameLiftDescribeGameSessionPlacement(); \
	friend struct Z_Construct_UClass_UGameLiftDescribeGameSessionPlacement_Statics; \
public: \
	DECLARE_CLASS(UGameLiftDescribeGameSessionPlacement, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/GameLiftClientSDK"), NO_API) \
	DECLARE_SERIALIZER(UGameLiftDescribeGameSessionPlacement)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftDescribeGameSessionPlacement(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftDescribeGameSessionPlacement) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftDescribeGameSessionPlacement); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftDescribeGameSessionPlacement); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftDescribeGameSessionPlacement(UGameLiftDescribeGameSessionPlacement&&); \
	NO_API UGameLiftDescribeGameSessionPlacement(const UGameLiftDescribeGameSessionPlacement&); \
public:


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API UGameLiftDescribeGameSessionPlacement(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API UGameLiftDescribeGameSessionPlacement(UGameLiftDescribeGameSessionPlacement&&); \
	NO_API UGameLiftDescribeGameSessionPlacement(const UGameLiftDescribeGameSessionPlacement&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, UGameLiftDescribeGameSessionPlacement); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(UGameLiftDescribeGameSessionPlacement); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(UGameLiftDescribeGameSessionPlacement)


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_PRIVATE_PROPERTY_OFFSET
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_231_PROLOG
#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_RPC_WRAPPERS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_INCLASS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_PRIVATE_PROPERTY_OFFSET \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_RPC_WRAPPERS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_INCLASS_NO_PURE_DECLS \
	UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h_234_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> GAMELIFTCLIENTSDK_API UClass* StaticClass<class UGameLiftDescribeGameSessionPlacement>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID UE4_ProjectZ_422_Plugins_GameLiftClientSDK_Source_GameLiftClientSDK_Public_GameLiftClientApi_h


#define FOREACH_ENUM_EACTIVATESTATUS(op) \
	op(EActivateStatus::ACTIVATE_Success) \
	op(EActivateStatus::ACTIVATE_NoGameLift) \
	op(EActivateStatus::ACTIVATE_NoSuccessCallback) \
	op(EActivateStatus::ACTIVATE_NoFailCallback) 

enum class EActivateStatus : uint8;
template<> GAMELIFTCLIENTSDK_API UEnum* StaticEnum<EActivateStatus>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
