// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZType.h"

DECLARE_LOG_CATEGORY_EXTERN(ProjectZ, Log, All);

#define ZLOG_CALLINFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))
#define ZLOG_S(Verbosity) UE_LOG(ProjectZ, Verbosity, TEXT("%s"), *ZLOG_CALLINFO)
#define ZLOG(Verbosity, Format, ...) UE_LOG(ProjectZ, Verbosity, TEXT("%s %s"), *ZLOG_CALLINFO, *FString::Printf(Format, ##__VA_ARGS__))

static const ECollisionChannel PROJECTILE = ECC_GameTraceChannel1;
static const ECollisionChannel WEAPON = ECC_GameTraceChannel2;
static const ECollisionChannel PROJECTILE_TRACE = ECC_GameTraceChannel3;
static const ECollisionChannel WEAPON_TRACE = ECC_GameTraceChannel4;
static const ECollisionChannel ZCHARACTER = ECC_GameTraceChannel5;
