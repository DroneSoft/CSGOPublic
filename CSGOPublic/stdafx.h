#pragma once

//This isnt precompiled.

#define WIN32_LEAN_AND_MEAN

#define _CRT_SECURE_NO_WARNINGS
#define _SCL_SECURE_NO_WARNINGS
#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS

#include	<windows.h>
#include	<stdio.h>
#include	<math.h>
#include	<Tlhelp32.h>
#include	<Shlwapi.h>
#include	<Shlobj.h>
#include	<time.h>
#include	<vector>
#include	<ddraw.h>
#include	<d3dx9.h>
#include	<d3d9.h>
#include	<D3dx9tex.h>
#include	<D3dx9shape.h>
#include	<detours.h>

#pragma warning(disable : 4244)
#pragma warning(disable : 4018)
#pragma warning(disable : 4800)
#pragma warning(disable : 4455)
#pragma warning(disable : 4305)
#pragma warning(disable : 4996)
#pragma warning(disable : 4138)

#pragma comment( lib, "detours.lib")

#pragma comment	( lib, "d3d9.lib" )
#pragma comment	( lib, "d3dx9.lib" )
#pragma comment ( lib, "dxguid.lib" )
#pragma comment ( lib, "Shlwapi.lib" )
#pragma comment ( lib, "winmm.lib" )

#include "GlobalDefs.h"
#include "VMTHook.h"
#include "CDrawing.h"
#include "Math.h"
#include "CDrone.h"
#include "D3D9Drawing.h"
#include "CMenu.h"

extern unsigned long SegoeUI;

DWORD WINAPI HackThread(PVOID pThreadModule);
void InitGameHooks();
void InitGameVars();
