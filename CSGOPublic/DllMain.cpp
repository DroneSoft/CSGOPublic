#include "stdafx.h"

#define SIZE_OF_MENU 2048

CreateInterfaceFn fnIClient = NULL;
CreateInterfaceFn fnIEngine = NULL;
CreateInterfaceFn fnIVGuiMat = NULL;
CreateInterfaceFn fnIVGui2 = NULL;

Color Red(200, 0, 0, 255);
Color Green(0, 200, 0, 255);
Color Blue(0, 0, 200, 255);
Color Yellow(200, 200, 0, 255);
Color White(255, 255, 255, 255);
Color Black(0, 0, 0, 255);

MEMORY_BASIC_INFORMATION mbi;

bool bOnce = false;
bool bRes = false;

typedef void(__thiscall *PaintTraverse)(void*, unsigned int, bool, bool);
typedef bool(__thiscall *CreateMove)(void*, float flInputSampleTime, CUserCmd *cmd);
typedef int(__stdcall* IN_KeyEvent)(void *, int eventcode, ButtonCode_t keynum, const char *pszCurrentBinding);
typedef HRESULT(WINAPI *DrawIndexedPrimitive)(LPDIRECT3DDEVICE9 device, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
typedef HRESULT(WINAPI *CreateQuery)(LPDIRECT3DDEVICE9 pDevice, D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery);
typedef HRESULT(WINAPI *Reset_t)(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters);
typedef HRESULT(WINAPI *EndScene_t)(LPDIRECT3DDEVICE9 pDevice);
typedef int(__stdcall *oTraceRay)(const Ray_t &ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace);
oTraceRay _TraceRay;

PaintTraverse _PaintTraverse;
CreateMove _CreateMove;
IN_KeyEvent _IN_KeyEvent;
DrawIndexedPrimitive _DrawIndexedPrimitive;
CreateQuery _CreateQuery;
EndScene_t _EndScene;
Reset_t _Reset;

IClientMode ** ClientMode;
CVMTHook* pCreateMoveHook;
CVMTHook* pD3D9DIPHook;

unsigned long SegoeUI;
bool DoOnce = true;
UINT Stride;

CDX9Canvas Canvas;
CCanvas* pCanvas;

var_type *vVars;

bool bDrawMenu = false;
CRITICAL_SECTION lpCritSection;

void RunClient(CUserCmd* pCmd)
{
	IClientEntity* pLocal = pClientEntList->GetClientEntity(pEngine->GetLocalPlayer());
	//do stuff
	//if (GetAsyncKeyState(VK_INSERT))
	//	printf_s("DroneSoft: Insert Pressed.");
	//gDrone->UpdateAim();

	D3DXVECTOR3 Punch = pLocal->GetPunchAngle();
	pCmd->viewangles -= Punch * 2.0f;

	if (pCmd->buttons & IN_JUMP)
		if (!(pLocal->GetFlags() & FL_ONGROUND))
			pCmd->buttons &= ~IN_JUMP;
}

bool __stdcall nCreateMove(float flInputSampleTime, CUserCmd *cmd)
{
	if (!cmd->command_number)
	{
		return true;
	}

	if (pEngine->IsInGame() && pEngine->IsConnected())
	{
		RunClient(cmd);
		return false;
	}
	return false;
}
void __fastcall HookedPaintTraverse(void* ecx, void* edx, int VGUIPanel, bool ForceRepaint, bool AllowForce)
{
	_PaintTraverse(ecx, VGUIPanel, ForceRepaint, AllowForce);

	if (!strcmp("MatSystemTopPanel", pPanel->GetName(VGUIPanel)))
	{
		pEngine->GetScreenSize(sScreenSize2.iWidth, sScreenSize2.iHeight);

		if (!bRes && (sScreenSize2.iWidth != sScreenSize.iWidth || sScreenSize2.iHeight != sScreenSize.iHeight))
		{
			sScreenSize.iWidth = sScreenSize2.iWidth;
			sScreenSize.iHeight = sScreenSize2.iHeight;
			bRes = true;
		}

		if (!bOnce)
		{
			Arial = pSurface->FontCreate();
			pSurface->SetFontGlyphSet(Arial, "Arial", 14, 500, 0, 0, FONTFLAG_OUTLINE);
			bOnce = true;
		} 

		if (bRes)
		{
			Arial = pSurface->FontCreate();
			pSurface->SetFontGlyphSet(Arial, "Arial", 14, 500, 0, 0, FONTFLAG_OUTLINE);
			bRes = false;
		}

		if (pEngine->IsInGame() && pEngine->IsConnected())
		{
			gDrone->UpdateDraw();
		}
	}
}

CreateInterfaceFn CaptureFactory(char *pszFactoryModule)
{
	CreateInterfaceFn fn = NULL;
	while (fn == NULL)
	{
		HMODULE hFactoryModule = GetModuleHandleA(pszFactoryModule);
		if (hFactoryModule)
		{
			fn = reinterpret_cast<CreateInterfaceFn>(GetProcAddress(hFactoryModule, "CreateInterface"));
		}
		Sleep(10);
	}
	return fn;
}

void *CaptureInterface(CreateInterfaceFn fn, char *pszInterfaceName)
{
	unsigned long *ptr = NULL;
	while (ptr == NULL)
	{
		ptr = reinterpret_cast<unsigned long*>(fn(pszInterfaceName, NULL));
		Sleep(10);
	}

	return ptr;
}
LPDIRECT3DTEXTURE9    texYellow, texRed, texGreen, texBlue;
HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32);
void InitTextures(IDirect3DDevice9 *m_pD3Ddev);

void InitTextures(IDirect3DDevice9 *m_pD3Ddev)
{
	GenerateTexture(m_pD3Ddev, &texRed, D3DCOLOR_ARGB(255, 200, 0, 0));
	GenerateTexture(m_pD3Ddev, &texGreen , D3DCOLOR_ARGB(255, 0, 200, 0));
	GenerateTexture(m_pD3Ddev, &texYellow, D3DCOLOR_ARGB(255, 200, 200, 0));
	GenerateTexture(m_pD3Ddev, &texBlue, D3DCOLOR_ARGB(255, 0, 0, 200));
}

HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{
	if (FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex, NULL)))
		return E_FAIL;

	WORD colour16 = ((WORD)((colour32 >> 28) & 0xF) << 12)
		| (WORD)(((colour32 >> 20) & 0xF) << 8)
		| (WORD)(((colour32 >> 12) & 0xF) << 4)
		| (WORD)(((colour32 >> 4) & 0xF) << 0);

	D3DLOCKED_RECT d3dlr;
	(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
	WORD *pDst16 = (WORD*)d3dlr.pBits;

	for (int xy = 0; xy < 8 * 8; xy++)
		*pDst16++ = colour16;

	(*ppD3Dtex)->UnlockRect(0);

	return S_OK;
}

HRESULT WINAPI nDrawIndexedPrimitive(LPDIRECT3DDEVICE9 device, D3DPRIMITIVETYPE PrimitiveType, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount)
{
	LPDIRECT3DVERTEXBUFFER9 StreamData;
	UINT OffsetInBytes;

	if (device->GetStreamSource(0, &StreamData, &OffsetInBytes, &Stride) == D3D_OK) //Crash at this line
	{
		if (StreamData != NULL)
			StreamData->Release();
	}
	
	if (ct_all)
	{
		DWORD dwOldZEnable = D3DZB_TRUE;
		device->SetTexture(0, texGreen);
		device->GetRenderState(D3DRS_ZENABLE, &dwOldZEnable);
		device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		_DrawIndexedPrimitive(device, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		device->SetRenderState(D3DRS_ZENABLE, dwOldZEnable);
		device->SetTexture(0, texBlue);
	}
	if (t_all)
	{
		DWORD dwOldZEnable = D3DZB_TRUE;
		device->SetTexture(0, texYellow);
		device->GetRenderState(D3DRS_ZENABLE, &dwOldZEnable);
		device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
		_DrawIndexedPrimitive(device, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
		device->SetRenderState(D3DRS_ZENABLE, dwOldZEnable);
		device->SetTexture(0, texRed);
	}


	return _DrawIndexedPrimitive(device, PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT WINAPI nCreateQuery(LPDIRECT3DDEVICE9 pDevice, D3DQUERYTYPE Type, IDirect3DQuery9** ppQuery)
{
	if (Type == D3DQUERYTYPE_OCCLUSION)
	{
		*ppQuery = new CFakeQuery;

		((CFakeQuery*)*ppQuery)->m_type = Type;

		return D3D_OK;
	}

	return _CreateQuery(pDevice, Type, ppQuery);
}

HRESULT WINAPI nEndScene(LPDIRECT3DDEVICE9 pDevice)
{
	EnterCriticalSection(&lpCritSection);

	if (DoOnce)
	{
		InitTextures(pDevice);
		DoOnce = false;
	}

	pCanvas->Reset();
	pCanvas->Init(pDevice);
	pCanvas->Begin();

	D3DVIEWPORT9 ViewPort;
	pDevice->GetViewport(&ViewPort);

	if(GetAsyncKeyState(VK_INSERT)&1)
	{
		bDrawMenu = !bDrawMenu;
	}

	if (bDrawMenu)
	{
		pCanvas->Text("Please draw...", 200, 200, 0xFFFFFFFF);
	}

	pCanvas->Fill(ViewPort.Width / 2 - 15, (ViewPort.Height / 2), 30, 2, D3DCOLOR_RGBA(102, 51, 153, 255));
	pCanvas->Fill((ViewPort.Width / 2), (ViewPort.Height / 2) - 15, 2, 30, D3DCOLOR_RGBA(102, 51, 153, 255));

	pCanvas->End();

	LeaveCriticalSection(&lpCritSection);
	return _EndScene(pDevice);
}

HRESULT WINAPI nReset(LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters)
{
	return _Reset(pDevice, pPresentationParameters);
}

DWORD WINAPI HackThread(PVOID pThreadModule)
{
	//Init a console for debugging purposes.  We can use printf for debugging.
	
	//if (!AllocConsole())
	//{
	//	return 0x0;
	//}
	//SetConsoleTitle("DroneSoft Overlay");

	//freopen("CONIN$", "rb", stdin);
	//freopen("CONOUT$", "wb", stdout);
	//freopen("CONOUT$", "wb", stdin);
	

	DWORD dwEngineModule = NULL;
	DWORD dwClientModule = NULL;
	DWORD dwVGUI2Module = NULL;
	DWORD dwMatSurfModule = NULL;
	DWORD dwD3D9Module = NULL;

	HMODULE hDLL = NULL;
	while (!hDLL)
	{
		hDLL = GetModuleHandleA("d3d9.dll");
		Sleep(100);
	}
	CloseHandle(hDLL);

	vVars = (var_type*)malloc(SIZE_OF_MENU);
	InitializeCriticalSection(&lpCritSection);

	LPDIRECT3D9(__stdcall*pDirect3DCreate9)(UINT) = (LPDIRECT3D9(__stdcall*)(UINT))GetProcAddress(hDLL, "Direct3DCreate9");

	LPDIRECT3D9 pD3D = pDirect3DCreate9(D3D_SDK_VERSION);

	D3DDISPLAYMODE d3ddm;
	HRESULT hRes = pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm);
	D3DPRESENT_PARAMETERS d3dpp;
	memset((void*)&d3dpp, 0, sizeof(d3dpp));
	d3dpp.Windowed = true;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.BackBufferFormat = d3ddm.Format;

	pCanvas = &Canvas;

	LPDIRECT3DDEVICE9 ppReturnedDeviceInterface;

	hRes = pD3D->CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		GetForegroundWindow(),
		D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_DISABLE_DRIVER_MANAGEMENT,
		&d3dpp, &ppReturnedDeviceInterface);

	pD3D->Release();


	unsigned long* pInterface;
	pInterface = (unsigned long*)*((unsigned long*)ppReturnedDeviceInterface);
	_Reset = (Reset_t)(DWORD)pInterface[16];
	_EndScene = (EndScene_t)(DWORD)pInterface[42];
	_CreateQuery = (CreateQuery)(DWORD)pInterface[118];
	_DrawIndexedPrimitive = (DrawIndexedPrimitive)(DWORD)pInterface[82];



	(VOID)DetourTransactionBegin();
	(VOID)DetourUpdateThread(GetCurrentThread());
	(VOID)DetourAttach(&(PVOID&)_Reset, nReset);
	(VOID)DetourTransactionCommit();

	(VOID)DetourTransactionBegin();
	(VOID)DetourUpdateThread(GetCurrentThread());
	(VOID)DetourAttach(&(PVOID&)_EndScene, nEndScene);
	(VOID)DetourTransactionCommit();

	(VOID)DetourTransactionBegin();
	(VOID)DetourUpdateThread(GetCurrentThread());
	(VOID)DetourAttach(&(PVOID&)_CreateQuery, nCreateQuery);
	(VOID)DetourTransactionCommit();

	(VOID)DetourTransactionBegin();
	(VOID)DetourUpdateThread(GetCurrentThread());
	(VOID)DetourAttach(&(PVOID&)_DrawIndexedPrimitive, nDrawIndexedPrimitive);
	(VOID)DetourTransactionCommit();

	do
	{
		dwEngineModule = (DWORD)GetModuleHandleA("engine.dll");
		dwClientModule = (DWORD)GetModuleHandleA("client.dll");
		dwVGUI2Module = (DWORD)GetModuleHandleA("vgui2.dll");
		dwMatSurfModule = (DWORD)GetModuleHandleA("vguimatsurface.dll");
		Sleep(100);
	} while (!dwEngineModule || !dwClientModule || !dwVGUI2Module || !dwMatSurfModule);
	
	//Acquire each dll's factory...
	fnIClient = CaptureFactory("client.dll");
	fnIEngine = CaptureFactory("engine.dll");
	fnIVGuiMat = CaptureFactory("vguimatsurface.dll");
	fnIVGui2 = CaptureFactory("vgui2.dll"); 

	//Get our interface pointer for each base interface.
	pClient = reinterpret_cast<IBaseClientDLL*>(CaptureInterface(fnIClient, "VClient017"));
	pEngine = reinterpret_cast<IVEngineClient*>(CaptureInterface(fnIEngine, "VEngineClient013"));
	pSurface = reinterpret_cast<ISurface*>(CaptureInterface(fnIVGuiMat, "VGUI_Surface031"));
	pClientEntList = reinterpret_cast<IClientEntityList*>(CaptureInterface(fnIClient, "VClientEntityList003"));
	pPanel = reinterpret_cast<IPanel*>(CaptureInterface(fnIVGui2, "VGUI_Panel009"));
	pEngineTrace = reinterpret_cast<IEngineTrace*>(CaptureInterface(fnIEngine, "EngineTraceClient004"));

	PDWORD pdwClientVMT = *reinterpret_cast< PDWORD_PTR* >(pClient);
	DWORD dwInitAddr = (DWORD)(pdwClientVMT[0]);
	for (DWORD dwIter = 0; dwIter <= 0xFF; dwIter++)
	{
		if (*(PBYTE)(dwInitAddr + dwIter - 1) == 0x51 && *(PBYTE)(dwInitAddr + dwIter) == 0xA3)
		{
			pGlobals = (CGlobalVarsBase*)*(PDWORD)*(PDWORD)(dwInitAddr + dwIter + 1);
			break;
		}
	}

	Sleep(5000);

	void** BaseClientDllVMT = *(void***)pClient;
	ClientMode = *(IClientMode***)((DWORD)BaseClientDllVMT[10] + 5);

	pCreateMoveHook = new CVMTHook(*(DWORD***)ClientMode);
	_CreateMove = (CreateMove)pCreateMoveHook->dwHookMethod((DWORD)nCreateMove, 24);

	pPanelHook = new CVMTHook((DWORD**)pPanel);
	_PaintTraverse = (PaintTraverse)pPanelHook->dwHookMethod((DWORD)HookedPaintTraverse, 41);

	//_TraceRay = (oTraceRay)((DWORD*)((DWORD)GetModuleHandle("engine.dll") + 0x223CE0));

	return 0x0;
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD fdwReason, LPVOID lpvReserved)
{
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		DisableThreadLibraryCalls(hMod);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HackThread, hMod, 0, NULL);
		break;
	case DLL_PROCESS_DETACH:
		DeleteCriticalSection(&lpCritSection);
		break;
	}
	
	return TRUE;
}