#pragma once

#ifndef __GLOBALDEFS_H__
#define __GLOBALDEFS_H__

#include "engine_crc32.h"

//interfaces.h pretty much c+p'd this from source engine
//rebuilds is a mix of community and me.

class IBaseClientDLL;
class IVEngineClient;
class IPanel;
class IClientEntityList;
class ISurface;
class IVModelInfo;
class CGlobalVarsBase;
class IEngineTrace;
class CInput;
class IMaterialSystem;
class IVModelRender;
class IGameMovement;
class IPrediction;
class IVEfx;
class HFont;
class IVPhysicsDebugOverlay;
class RecvTable;
class RecvProp;
class IHandleEntity;
class CBaseHandle;
class IClientNetworkable;
class IClientEntity;
class IClientThinkable;
class IClientUnknown;
class IClientNetworkable;
class ITraceFilter;
class CGameTrace;
typedef CGameTrace trace_t;

typedef void*	(*CreateClientClassFn)(int entnum, int serialNum);
typedef void*	(*CreateEventFn)();
typedef void*	(*CreateInterfaceFn)(const char *pName, int *pReturnCode);

extern CreateInterfaceFn fnIClient;
extern CreateInterfaceFn fnIEngine;
extern CreateInterfaceFn fnIVGuiMat;
extern CreateInterfaceFn fnIVGui2;

//Globals
#define M_PI 3.14159265358979323846f
#define ANG_CLIP( ang )		if( ang > 180.0f ) { ang -= 360.0f; } else if( ang <- 180.0f ) { ang += 360.0f; }
#define	GENTITYNUM_BITS		10
#define	MAX_GENTITIES		( 1 << GENTITYNUM_BITS )
#define	FL_ONGROUND (1<<0)

#define INTERFACEVERSION_PLAYERINFOMANAGER			"PlayerInfoManager002"

struct matrix3x4_t;
class IPlayerInfoManager;
class IPlayerInfo;
class CBotCmd;

class IClientCollideable;

// Note: These flags need to match the bspfile.h DISPTRI_TAG_* flags.
#define DISPSURF_FLAG_SURFACE		(1<<0)
#define DISPSURF_FLAG_WALKABLE		(1<<1)
#define DISPSURF_FLAG_BUILDABLE		(1<<2)
#define DISPSURF_FLAG_SURFPROP1		(1<<3)
#define DISPSURF_FLAG_SURFPROP2		(1<<4)


class IPlayerInfoManager
{
public:
	//virtual IPlayerInfo *GetPlayerInfo(edict_t *pEdict) = 0;
	virtual CGlobalVarsBase *GetGlobalVars() = 0;
};


struct matrix3x4_t
{
	matrix3x4_t() {}
	matrix3x4_t(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23)
	{
		m_flMatVal[0][0] = m00;	m_flMatVal[0][1] = m01; m_flMatVal[0][2] = m02; m_flMatVal[0][3] = m03;
		m_flMatVal[1][0] = m10;	m_flMatVal[1][1] = m11; m_flMatVal[1][2] = m12; m_flMatVal[1][3] = m13;
		m_flMatVal[2][0] = m20;	m_flMatVal[2][1] = m21; m_flMatVal[2][2] = m22; m_flMatVal[2][3] = m23;
	}
	void Init(const D3DXVECTOR3& xAxis, const D3DXVECTOR3& yAxis, const D3DXVECTOR3& zAxis, const D3DXVECTOR3 &vecOrigin)
	{
		m_flMatVal[0][0] = xAxis.x; m_flMatVal[0][1] = yAxis.x; m_flMatVal[0][2] = zAxis.x; m_flMatVal[0][3] = vecOrigin.x;
		m_flMatVal[1][0] = xAxis.y; m_flMatVal[1][1] = yAxis.y; m_flMatVal[1][2] = zAxis.y; m_flMatVal[1][3] = vecOrigin.y;
		m_flMatVal[2][0] = xAxis.z; m_flMatVal[2][1] = yAxis.z; m_flMatVal[2][2] = zAxis.z; m_flMatVal[2][3] = vecOrigin.z;
	}

	matrix3x4_t(const D3DXVECTOR3& xAxis, const D3DXVECTOR3& yAxis, const D3DXVECTOR3& zAxis, const D3DXVECTOR3 &vecOrigin)
	{
		Init(xAxis, yAxis, zAxis, vecOrigin);
	}
	float *operator[](int i)
	{
		return m_flMatVal[i];
	}
	const float *operator[](int i) const
	{
		return m_flMatVal[i];
	}
	float *Base()
	{
		return &m_flMatVal[0][0];
	}
	const float *Base() const
	{
		return &m_flMatVal[0][0];
	}

	float m_flMatVal[3][4];
};

template< typename Function > Function call_vfunc(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];
	return (Function)(dwAddress);
}

enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC = 0x001,
	FONTFLAG_UNDERLINE = 0x002,
	FONTFLAG_STRIKEOUT = 0x004,
	FONTFLAG_SYMBOL = 0x008,
	FONTFLAG_ANTIALIAS = 0x010,
	FONTFLAG_GAUSSIANBLUR = 0x020,
	FONTFLAG_ROTARY = 0x040,
	FONTFLAG_DROPSHADOW = 0x080,
	FONTFLAG_ADDITIVE = 0x100,
	FONTFLAG_OUTLINE = 0x200,
	FONTFLAG_CUSTOM = 0x400,
	FONTFLAG_BITMAP = 0x800,
};

class Color
{
public:
	// constructors
	Color()
	{
		*((int *)this) = 0;
	}
	Color(int r, int g, int b)
	{
		SetColor(r, g, b, 0);
	}
	Color(int r, int g, int b, int a)
	{
		SetColor(r, g, b, a);
	}
	void SetColor(int r, int g, int b, int a = 0)
	{
		_color[0] = (unsigned char)r;
		_color[1] = (unsigned char)g;
		_color[2] = (unsigned char)b;
		_color[3] = (unsigned char)a;
	}

	void GetColor(int &r, int &g, int &b, int &a) const
	{
		r = _color[0];
		g = _color[1];
		b = _color[2];
		a = _color[3];
	}

	void SetRawColor(int color32)
	{
		*((int *)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int *)this);
	}

	inline int r() const	{ return _color[0]; }
	inline int g() const	{ return _color[1]; }
	inline int b() const	{ return _color[2]; }
	inline int a() const	{ return _color[3]; }

	unsigned char &operator[](int index)
	{
		return _color[index];
	}

	const unsigned char &operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const Color &rhs) const
	{
		return (*((int *)this) == *((int *)&rhs));
	}

	bool operator != (const Color &rhs) const
	{
		return !(operator==(rhs));
	}

	Color &operator=(const Color &rhs)
	{
		SetRawColor(rhs.GetRawColor());
		return *this;
	}

private:
	unsigned char _color[4];
};
struct Ray_t
{
	D3DXVECTOR3  m_Start;	// starting point, centered within the extents
	D3DXVECTOR3  m_Delta;	// direction + length of the ray
	D3DXVECTOR3  m_StartOffset;	// Add this to m_Start to get the actual ray start
	D3DXVECTOR3  m_Extents;	// Describes an axis aligned box extruded along a ray
	bool	m_IsRay;	// are the extents zero?
	bool	m_IsSwept;	// is delta != 0?
private:
};

class player_info_t
{
public:
	char unknown0[0x10];
	char name[32];
	char unknown2[0x200];
};

class CGlobalVarsBase
{
public:
	CGlobalVarsBase(bool bIsClient);
	bool IsClient() const;
	int GetNetworkBase(int nTick, int nEntity);
	float			realtime;
	int				framecount;
	float			absoluteframetime;
	float			curtime;
	float			frametime;
	int				maxClients;
	int				tickcount;
	float			interval_per_tick;
	float			interpolation_amount;
	int				simTicksThisFrame;
	int				network_protocol;
	void *pSaveData;
	bool			m_bClient;
	int				nTimestampNetworkingBase;
	int				nTimestampRandomizeWindow;
};
class IBaseClientDLL
{
public:
	void CreateMove(int sequence_number, float frametime, bool active)
	{
		typedef void(__thiscall* oMove)(PVOID, int, float, bool);
		return call_vfunc< oMove >(this, 21)(this, sequence_number, frametime, active);
	}
};

class IVEngineClient
{
public:
	void GetScreenSize(int& width, int& height)
	{
		typedef void(__thiscall* oGetScreenSize)(PVOID, int&, int&);
		return call_vfunc< oGetScreenSize >(this, 5)(this, width, height);
	}
	void GetPlayerInfo(int ent_num, player_info_t *pinfo)
	{
		typedef void(__thiscall* oGetPlayerInfo)(void*, int, player_info_t*);
		return call_vfunc< oGetPlayerInfo >(this, 8)(this, ent_num, pinfo);
	}
	int GetLocalPlayer()
	{
		typedef int(__thiscall* oLocal)(PVOID);
		return call_vfunc< oLocal >(this, 12)(this);
	}
	float Time()
	{
		typedef float(__thiscall* oTime)(PVOID);
		return call_vfunc< oTime >(this, 14)(this);
	}
	void SetViewAngles(D3DXVECTOR3& vAngles)
	{
		typedef void(__thiscall* oSetViewAngles)(PVOID, D3DXVECTOR3&);
		return call_vfunc< oSetViewAngles >(this, 19)(this, vAngles);
	}
	int GetMaxClients()
	{
		typedef bool(__thiscall* oGetMaxClients)(PVOID);
		return call_vfunc< oGetMaxClients >(this, 20)(this);
	}
	bool IsConnected()
	{
		typedef bool(__thiscall* oGetScreenSize)(PVOID);
		return call_vfunc< oGetScreenSize >(this, 27)(this);
	}
	bool IsInGame()
	{
		typedef bool(__thiscall* oLocal)(PVOID);
		return call_vfunc< oLocal >(this, 26)(this);
	}
	const D3DXMATRIX& WorldToScreenMatrix()
	{
		typedef const D3DXMATRIX& (__thiscall* oWorldToScreenMatrix)(PVOID);
		return call_vfunc< oWorldToScreenMatrix >(this, 37)(this);
	}
};

class CBaseTrace
{
public:

	// Displacement flags tests.
	bool IsDispSurface(void)				{ return ((dispFlags & DISPSURF_FLAG_SURFACE) != 0); }
	bool IsDispSurfaceWalkable(void)		{ return ((dispFlags & DISPSURF_FLAG_WALKABLE) != 0); }
	bool IsDispSurfaceBuildable(void)		{ return ((dispFlags & DISPSURF_FLAG_BUILDABLE) != 0); }
	bool IsDispSurfaceProp1(void)			{ return ((dispFlags & DISPSURF_FLAG_SURFPROP1) != 0); }
	bool IsDispSurfaceProp2(void)			{ return ((dispFlags & DISPSURF_FLAG_SURFPROP2) != 0); }

public:

	// these members are aligned!!
	D3DXVECTOR3			startpos;				// start position
	D3DXVECTOR3				endpos;					// final position
	DWORD		plane;					// surface normal at impact

	float			fraction;				// time completed, 1.0 = didn't hit anything

	int				contents;				// contents on other side of surface hit
	unsigned short	dispFlags;				// displacement flags for marking surfaces with data

	bool			allsolid;				// if true, plane is not valid
	bool			startsolid;				// if true, the initial point was in a solid area

	CBaseTrace() {}

private:
	// No copy constructors allowed
	CBaseTrace(const CBaseTrace& vOther);
};

//-----------------------------------------------------------------------------
// Purpose: A trace is returned when a box is swept through the world
// NOTE: eventually more of this class should be moved up into the base class!!
//-----------------------------------------------------------------------------
class CGameTrace : public CBaseTrace
{
public:

	// Returns true if hEnt points at the world entity.
	// If this returns true, then you can't use GetHitBoxIndex().
	bool DidHitWorld() const;

	// Returns true if we hit something and it wasn't the world.
	bool DidHitNonWorldEntity() const;

	// Gets the entity's network index if the trace has hit an entity.
	// If not, returns -1.
	int GetEntityIndex() const;

	// Returns true if there was any kind of impact at all
	bool DidHit() const;

public:

	float		fractionleftsolid;		// time we left a solid, only valid if we started in solid
	DWORD	surface;				// surface hit (impact surface)

	int			hitgroup;				// 0 == generic, non-zero is specific body part
	short		physicsbone;			// physics bone hit by trace in studio


	IClientEntity *m_pEnt;

	// NOTE: this member is overloaded.
	// If hEnt points at the world entity, then this is the static prop index.
	// Otherwise, this is the hitbox index.
	int			hitbox;					// box hit by trace in studio

	CGameTrace() {}

private:
	// No copy constructors allowed
	CGameTrace(const CGameTrace& vOther);
};


class ISurface
{
public:
	void DrawSetColor(Color col)
	{
		typedef void(__thiscall* oDrawSetColor)(PVOID, Color);
		return call_vfunc< oDrawSetColor >(this, 14)(this, col);
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* oDrawSetColor)(PVOID, int, int, int, int);
		return call_vfunc< oDrawSetColor >(this, 16)(this, x0, y0, x1, y1);
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* oDrawOutlinedRect)(PVOID, int, int, int, int);
		return call_vfunc< oDrawOutlinedRect >(this, 18)(this, x0, y0, x1, y1);
	}
	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void(__thiscall* oDrawLine)(PVOID, int, int, int, int);
		return call_vfunc< oDrawLine >(this, 19)(this, x0, y0, x1, y1);
	}
	void DrawSetTextFont(unsigned long hFont)
	{
		typedef void(__thiscall* oDrawSetTextFont)(PVOID, unsigned long);
		return call_vfunc< oDrawSetTextFont >(this, 23)(this, hFont);
	}
	void DrawSetTextColor(Color col)
	{
		typedef void(__thiscall* oDrawSetTextColor)(PVOID, Color);
		return call_vfunc< oDrawSetTextColor >(this, 24)(this, col);
	}
	void DrawSetTextPos(int x, int y)
	{
		typedef void(__thiscall* oDrawSetTextPos)(PVOID, int, int);
		return call_vfunc< oDrawSetTextPos >(this, 26)(this, x, y);
	}
	void DrawPrintText(wchar_t *text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT)
	{
		typedef void(__thiscall* oDrawPrintText)(PVOID, wchar_t*, int, FontDrawType_t);
		return call_vfunc< oDrawPrintText >(this, 28)(this, text, textLen, drawType);
	}
	unsigned long FontCreate()
	{
		typedef unsigned long(__thiscall* oCreateFont)(PVOID);
		return call_vfunc< oCreateFont >(this, 71)(this);
	}
	bool SetFontGlyphSet(unsigned long font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0)
	{
		typedef bool(__thiscall* oCreateFont)(PVOID, unsigned long, const char *, int, int, int, int, int, int, int);
		return call_vfunc< oCreateFont >(this, 72)(this, font, windowsFontName, tall, weight, blur, scanlines, flags, nRangeMin, nRangeMax);
	}
	bool GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall)
	{
		typedef bool(__thiscall* oGetTextSize)(PVOID, unsigned long, const wchar_t*, int&, int&);
		return call_vfunc< oGetTextSize >(this, 79)(this, font, text, wide, tall);
	}
};//Size=0x0004(4)
class IClientRenderable
{
public:
	virtual void*					GetIClientUnknown() = 0;
	virtual D3DXVECTOR3 const&			GetRenderOrigin(void) = 0;
	virtual D3DXVECTOR3 const&			GetRenderAngles(void) = 0;
	virtual bool					ShouldDraw(void) = 0;
	virtual bool					IsTransparent(void) = 0;
	virtual bool					UsesPowerOfTwoFrameBufferTexture() = 0;
	virtual bool					UsesFullFrameBufferTexture() = 0;
	virtual void					GetShadowHandle() const = 0;
	virtual void*					RenderHandle() = 0;
	virtual const void*/*model_t* */GetModel() const = 0;
	virtual int						DrawModel(int flags) = 0;
	virtual int						GetBody() = 0;
	virtual void					ComputeFxBlend() = 0;

	bool SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		typedef bool(__thiscall* oSetupBones)(PVOID, matrix3x4_t*, int, int, float);
		return call_vfunc< oSetupBones>(this, 13)(this, pBoneToWorldOut, nMaxBones, boneMask, currentTime);
	}
};
class IClientNetworkable
{
public:
	virtual IClientUnknown*	GetIClientUnknown() = 0;
	virtual void			Release() = 0;
	virtual void*			GetClientClass() = 0;// FOR NETVARS FIND YOURSELF ClientClass* stuffs
	virtual void			NotifyShouldTransmit( /* ShouldTransmitState_t state*/) = 0;
	virtual void			OnPreDataChanged( /*DataUpdateType_t updateType*/) = 0;
	virtual void			OnDataChanged( /*DataUpdateType_t updateType*/) = 0;
	virtual void			PreDataUpdate( /*DataUpdateType_t updateType*/) = 0;
	virtual void			PostDataUpdate( /*DataUpdateType_t updateType*/) = 0;
	virtual void			unknown();
	virtual bool			IsDormant(void) = 0;
	virtual int				Index(void) const = 0;
	virtual void			ReceiveMessage(int classID /*, bf_read &msg*/) = 0;
	virtual void*			GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities(void) = 0;
};
class IClientUnknown
{
public:
	virtual IClientCollideable*		GetCollideable() = 0;
	virtual IClientNetworkable*	GetClientNetworkable() = 0;
	virtual IClientRenderable*	GetClientRenderable() = 0;
	virtual IClientEntity*		GetIClientEntity() = 0;
	virtual IClientEntity*		GetBaseEntity() = 0;
	virtual IClientThinkable*	GetClientThinkable() = 0;
};
class IClientThinkable
{
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual void*				GetThinkHandle() = 0;
	virtual void				SetThinkHandle(void* hThink) = 0;
	virtual void				Release() = 0;
};
class __declspec (novtable)IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
public:
	virtual void			Release(void) = 0;
	virtual void			blahblahpad(void) = 0;
	virtual D3DXVECTOR3&	GetAbsOrigin(void) const = 0;//in broken place use GetOrigin Below
	virtual const D3DXVECTOR3&	GetAbsAngles(void) const = 0;

	D3DXVECTOR3 GetOrigin()
	{
		return *reinterpret_cast< D3DXVECTOR3* >((DWORD)this + (DWORD)0xA0);
	}
	int GetTeamNum()
	{
		return *reinterpret_cast< int* >((DWORD)this + (DWORD)0xF0);
	}
	int GetFlags()
	{
		return *reinterpret_cast< int* >((DWORD)this + (DWORD)0x100);
	}
	int GetHealth()
	{
		return *reinterpret_cast< int* >((DWORD)this + (DWORD)0xFC);
	}
	int GetMaxHealth()
	{
		return *reinterpret_cast< int* >((DWORD)this + (DWORD)0x264);
	}
	int GetLifeState()
	{
		return *reinterpret_cast< int* >((DWORD)this + (DWORD)0x25B);
	}
	bool isHostage()
	{
		return *reinterpret_cast< int *>((DWORD)this + (DWORD)0xD);
	}
	bool GetHealthFraction(int ent_num)
	{
		typedef bool(__stdcall* isDorm)(int);
		return call_vfunc< isDorm>(this, 107)(ent_num);
	}
	bool GetDormant()
	{
		return *reinterpret_cast<int*>((DWORD)this + (DWORD)0xE9);
	}
	D3DXVECTOR3 GetPunchAngle()
	{
		return *reinterpret_cast< D3DXVECTOR3* >((DWORD)this + (DWORD)+0x13E8);
	}
};

class IClientEntityList
{
public:
	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual IClientEntity *		GetClientEntity(int entnum);
	virtual IClientEntity *		GetClientEntityFromHandle(HANDLE hEnt) = 0;
	virtual int					NumberOfEntities(bool bIncludeNonNetworkable) = 0;
	virtual int					GetHighestEntityIndex(void);
	virtual void				SetMaxEntities(int maxents);
	virtual int					GetMaxEntities();
};
class IPanel
{
public:
	const char *GetName(int vguiPanel)
	{
		typedef const char *(__thiscall* oGetName)(PVOID, int);
		return call_vfunc< oGetName >(this, 36)(this, vguiPanel);
	}
	void PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce = true)
	{
		typedef void(__thiscall* oPaintTraverse)(PVOID, unsigned int, bool, bool);
		return call_vfunc< oPaintTraverse >(this, 41)(this, vguiPanel, forceRepaint, allowForce);
	} // you dont need this
};
class CUserCmd
{
public:
	virtual ~CUserCmd();//0x00
	int		command_number;//0x04
	int		tick_count;//0x08
	D3DXVECTOR3 viewangles;//0x0C
	D3DXVECTOR3	aimdirection;
	float	forwardmove;
	float	sidemove;
	float	upmove;
	int		buttons; //0x30
	BYTE    impulse; //0x35
	int		weaponselect;//0x36	
	int		weaponsubtype;//0x3A
	int		random_seed; //0x3E
	short	mousedx;//0x42		
	short	mousedy;//0x43		
	bool	hasbeenpredicted;
	D3DXVECTOR3 headangles;
	D3DXVECTOR3	headoffset;

	inline CRC32_t GetChecksum(void) const
	{
		CRC32_t crc;

		CRC32_Init(&crc);
		CRC32_ProcessBuffer(&crc, &command_number, sizeof(command_number));
		CRC32_ProcessBuffer(&crc, &tick_count, sizeof(tick_count));
		CRC32_ProcessBuffer(&crc, &viewangles, sizeof(viewangles));
		CRC32_ProcessBuffer(&crc, &aimdirection, sizeof(aimdirection));
		CRC32_ProcessBuffer(&crc, &forwardmove, sizeof(forwardmove));
		CRC32_ProcessBuffer(&crc, &sidemove, sizeof(sidemove));
		CRC32_ProcessBuffer(&crc, &upmove, sizeof(upmove));
		CRC32_ProcessBuffer(&crc, &buttons, sizeof(buttons));
		CRC32_ProcessBuffer(&crc, &impulse, sizeof(impulse));
		CRC32_ProcessBuffer(&crc, &weaponselect, sizeof(weaponselect));
		CRC32_ProcessBuffer(&crc, &weaponsubtype, sizeof(weaponsubtype));
		CRC32_ProcessBuffer(&crc, &random_seed, sizeof(random_seed));
		CRC32_ProcessBuffer(&crc, &mousedx, sizeof(mousedx));
		CRC32_ProcessBuffer(&crc, &mousedy, sizeof(mousedy));
		CRC32_ProcessBuffer(&crc, &hasbeenpredicted, sizeof(hasbeenpredicted));
		CRC32_ProcessBuffer(&crc, &headangles, sizeof(headangles));
		CRC32_ProcessBuffer(&crc, &headoffset, sizeof(headoffset));
		CRC32_Final(&crc);

		return crc;
	}
};
class ITraceFilter
{
public:
	virtual bool ShouldHitEntity(IHandleEntity *pEntity, int contentsMask) = 0;
	virtual void GetTraceType() const = 0; //TraceType_T -> ecx
};

class CInput
{
public:
	virtual		void		Init_All(void);
	virtual		void		Shutdown_All(void);
	virtual		int			GetButtonBits(int);
	virtual		void		CreateMove(int sequence_number, float input_sample_frametime, bool active);
	virtual		void		ExtraMouseSample(float frametime, bool active);
	virtual		bool		WriteUsercmdDeltaToBuffer( /*bf_write*/int *buf, int from, int to, bool isnewcommand);
	virtual		void		EncodeUserCmdToBuffer( /*bf_write&*/ int buf, int slot);
	virtual		void		DecodeUserCmdFromBuffer( /*bf_read&*/int buf, int slot);
	virtual		CUserCmd	*GetUserCmd(int something, int sequence_number);

	class CVerifiedUserCmd
	{
	public:
		CUserCmd        m_cmd;
		CRC32_t         m_crc;
	};

	CUserCmd         *m_pCommands;
	CVerifiedUserCmd *m_pVerifiedCommands;
};

enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up 
	MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,
};

enum TraceType_t
{
	TRACE_EVERYTHING = 0,
	TRACE_WORLD_ONLY,				// NOTE: This does *not* test static props!!!
	TRACE_ENTITIES_ONLY,			// NOTE: This version will *not* test static props
	TRACE_EVERYTHING_FILTER_PROPS,	// NOTE: This version will pass the IHandleEntity for props through the filter, unlike all other filters
};

enum PlayerControls
{
	IN_ATTACK = (1 << 0),
	IN_JUMP = (1 << 1),
	IN_DUCK = (1 << 2),
	IN_FORWARD = (1 << 3),
	IN_BACK = (1 << 4),
	IN_USE = (1 << 5),
	IN_CANCEL = (1 << 6),
	IN_LEFT = (1 << 7),
	IN_RIGHT = (1 << 8),
	IN_MOVELEFT = (1 << 9),
	IN_MOVERIGHT = (1 << 10),
	IN_ATTACK2 = (1 << 11),
	IN_RUN = (1 << 12),
	IN_RELOAD = (1 << 13),
	IN_ALT1 = (1 << 14),
	IN_ALT2 = (1 << 15),
	IN_SCORE = (1 << 16),	// Used by client.dll for when scoreboard is held down
	IN_SPEED = (1 << 17),	// Player is holding the speed key
	IN_WALK = (1 << 18),	// Player holding walk key
	IN_ZOOM = (1 << 19),	// Zoom key for HUD zoom
	IN_WEAPON1 = (1 << 20),	// weapon defines these bits
	IN_WEAPON2 = (1 << 21),	// weapon defines these bits
	IN_BULLRUSH = (1 << 22),
};

extern IBaseClientDLL*			pClient;
extern IVEngineClient*			pEngine;
extern IPanel*					pPanel;
extern IClientEntityList*		pClientEntList;
extern ISurface*				pSurface;
extern CGlobalVarsBase*			pGlobals;
extern IEngineTrace*			pEngineTrace;
extern unsigned long			espfont;

class CFakeQuery : public IDirect3DQuery9
{
public:
	HRESULT WINAPI QueryInterface(REFIID riid, void** ppvObj)
	{
		return D3D_OK;
	}

	ULONG WINAPI AddRef()
	{
		m_ref++;

		return m_ref;
	}

	ULONG WINAPI Release()
	{
		return 1;
	}

	HRESULT WINAPI GetDevice(IDirect3DDevice9 **ppDevice)
	{
		return D3D_OK;
	}

	D3DQUERYTYPE WINAPI GetType()
	{
		return m_type;
	}

	DWORD WINAPI GetDataSize()
	{
		return sizeof(DWORD);
	}

	HRESULT    WINAPI Issue(DWORD dwIssueFlags)
	{
		return D3D_OK;
	}

	HRESULT WINAPI GetData(void* pData, DWORD dwSize, DWORD dwGetDataFlags)
	{
		DWORD* pdwData = (DWORD*)pData;

		*pdwData = 500; //500 pixels visible

		return D3D_OK;
	}

	int                m_ref;
	D3DQUERYTYPE    m_type;
};

//////////////////////
//		CT 			//
//////////////////////


//IDF
#define ct_idf_legs	((Stride==32) && (NumVertices==2118) && (primCount==3354))
#define ct_idf_head1 ((Stride==32) && (NumVertices==1761) && (primCount==2681))
#define ct_idf_head2 ((Stride==32) && (NumVertices==1677) && (primCount==2713))
#define ct_idf_body ((Stride==32) && (NumVertices==4523) && (primCount==6700))

#define ct_idf ct_idf_legs || ct_idf_head1 || ct_idf_head2 || ct_idf_body


//SAS
#define ct_sas_legs ((Stride==32) && (NumVertices==4442) && (primCount==5475))
#define ct_sas_head ((Stride==32) && (NumVertices==1215) && (primCount==1749))
#define ct_sas_body ((Stride==32) && (NumVertices==5443) && (primCount==8008))

#define ct_sas ct_sas_legs || ct_sas_head || ct_sas_body


//FBI
#define ct_fbi_legs ((Stride==32) && (NumVertices==1646) && (primCount==2714))
#define ct_fbi_body ((Stride==32) && (NumVertices==5395) && (primCount==8657))
#define ct_fbi_head1 ((Stride==32) && (NumVertices==1459) && (primCount==2566))
#define ct_fbi_head2 ((Stride==32) && (NumVertices==2223) && (primCount==3672))
#define ct_fbi_head3 ((Stride==32) && (NumVertices==2778) && (primCount==4244))
#define ct_fbi_head4 ((Stride==32) && (NumVertices==993) && (primCount==1722))
#define ct_fbi_head5 ((Stride==32) && (NumVertices==2957) && (primCount==4234))

#define ct_fbi ct_fbi_legs || ct_fbi_body || ct_fbi_head1 || ct_fbi_head2 || ct_fbi_head3 || ct_fbi_head4 || ct_fbi_head5


//SWAT
#define ct_swat_legs ((Stride==32) && (NumVertices==3650) && (primCount==4030))
#define ct_swat_body1 ((Stride==32) && (NumVertices==6016) && (primCount==8022))
#define ct_swat_body2 ((Stride==32) && (NumVertices==5675) && (primCount==7713))
#define ct_swat_body3 ((Stride==32) && (NumVertices==6112) && (primCount==8261))
#define ct_swat_body4 ((Stride==32) && (NumVertices==6777) && (primCount==8930))
#define ct_swat_body5 ((Stride==32) && (NumVertices==6087) && (primCount==8226))
#define ct_swat_head1 ((Stride==32) && (NumVertices==1883) && (primCount==2986))
#define ct_swat_head2 ((Stride==32) && (NumVertices==1917) && (primCount==3004))
#define ct_swat_head3 ((Stride==32) && (NumVertices==1835) && (primCount==2898))
#define ct_swat_head4 ((Stride==32) && (NumVertices==1980) && (primCount==3074))
#define ct_swat_head5 ((Stride==32) && (NumVertices==1834) && (primCount==2898))

#define ct_swat ct_swat_legs || ct_swat_body1 || ct_swat_body2 || ct_swat_body3 || ct_swat_body4 || ct_swat_body5 || ct_swat_head1 || ct_swat_head2 || ct_swat_head3 || ct_swat_head4 || ct_swat_head5


//GSG9
#define ct_gsg9_legs ((Stride==32) && (NumVertices==1410) && (primCount==2352))
#define ct_gsg9_body ((Stride==32) && (NumVertices==3301) && (primCount==5424))
#define ct_gsg9_head1 ((Stride==32) && (NumVertices==2113) && (primCount==3391))
#define ct_gsg9_head2 ((Stride==32) && (NumVertices==2157) && (primCount==3461))
#define ct_gsg9_head3 ((Stride==32) && (NumVertices==2151) && (primCount==3459))

#define ct_gsg9 ct_gsg9_legs || ct_gsg9_body || ct_gsg9_head1 || ct_gsg9_head2 || ct_gsg9_head3


//GIGN
#define ct_gign_legs ((Stride==32) && (NumVertices==1258) && (primCount==1950))
#define ct_gign_body ((Stride==32) && (NumVertices==4854) && (primCount==7410))
#define ct_gign_head1 ((Stride==32) && (NumVertices==1645) && (primCount==2568))
#define ct_gign_head2 ((Stride==32) && (NumVertices==1611) && (primCount==2522))
#define ct_gign_head3 ((Stride==32) && (NumVertices==1601) && (primCount==2502))

#define ct_gign ct_gign_legs || ct_gign_body || ct_gign_head1 || ct_gign_head2 || ct_gign_head3


//SEAL
#define ct_seal_legs1 ((Stride==32) && (NumVertices==2924) && (primCount==3540))
#define ct_seal_legs2 ((Stride==32) && (NumVertices==2923) && (primCount==3540))
#define ct_seal_body1 ((Stride==32) && (NumVertices==5547) && (primCount==8003))
#define ct_seal_body2 ((Stride==32) && (NumVertices==5524) && (primCount==8106))
#define ct_seal_body3 ((Stride==32) && (NumVertices==5680) && (primCount==8166))
#define ct_seal_body4 ((Stride==32) && (NumVertices==5334) && (primCount==7910))
#define ct_seal_body5 ((Stride==32) && (NumVertices==5436) && (primCount==8003))
#define ct_seal_head1 ((Stride==32) && (NumVertices==2539) && (primCount==3527))
#define ct_seal_head2 ((Stride==32) && (NumVertices==3137) && (primCount==3948))
#define ct_seal_head3 ((Stride==32) && (NumVertices==3245) && (primCount==4158))
#define ct_seal_head4 ((Stride==32) && (NumVertices==2963) && (primCount==3792))
#define ct_seal_head5 ((Stride==32) && (NumVertices==4084) && (primCount==4697))

#define ct_seal ct_seal_legs1 || ct_seal_legs2 || ct_seal_body1 || ct_seal_body2 || ct_seal_body3 || ct_seal_body4 || ct_seal_body5 || ct_seal_head1 || ct_seal_head2 || ct_seal_head3 || ct_seal_head4 || ct_seal_head5

//////////////////////
//		T 			//
//////////////////////


//ELITE CREW
#define t_elite1_legs ((Stride==32) && (NumVertices==2531) && (primCount==3888))
#define t_elite1_head ((Stride==32) && (NumVertices==1134) && (primCount==2024))
#define t_elite1_body ((Stride==32) && (NumVertices==3606) && (primCount==5856))
#define t_elite2_legs ((Stride==32) && (NumVertices==1889) && (primCount==3088))
#define t_elite2_head ((Stride==32) && (NumVertices==1310) && (primCount==2302))
#define t_elite2_body ((Stride==32) && (NumVertices==3816) && (primCount==5930))
#define t_elite3_legs ((Stride==32) && (NumVertices==2416) && (primCount==3778))
#define t_elite3_head ((Stride==32) && (NumVertices==1430) && (primCount==2422))
#define t_elite3_body ((Stride==32) && (NumVertices==2587) && (primCount==4334))
#define t_elite4_legs ((Stride==32) && (NumVertices==2138) && (primCount==3424))
#define t_elite4_head ((Stride==32) && (NumVertices==1352) && (primCount==2268))
#define t_elite5_legs ((Stride==32) && (NumVertices==1986) && (primCount==3044))
#define t_elite5_head ((Stride==32) && (NumVertices==1372) && (primCount==2286))
#define t_elite5_body ((Stride==32) && (NumVertices== 3162) && (primCount==5182))


#define t_elite t_elite1_legs || t_elite1_head || t_elite1_body || t_elite2_legs || t_elite2_head || t_elite2_body || t_elite3_legs || t_elite3_head || t_elite3_body || t_elite4_legs || t_elite4_head || t_elite5_legs || t_elite5_head || t_elite5_body


//Separatist
#define t_separatist_legs ((Stride==32) && (NumVertices==2536) && (primCount==4196))
#define t_separatist_head ((Stride==32) && (NumVertices==1106) && (primCount==2018))
#define t_separatist_body ((Stride==32) && (NumVertices==3210) && (primCount==5382))

#define t_separatist t_separatist_legs || t_separatist_head || t_separatist_body


//Anarchist
#define t_anarchist_legs ((Stride==32) && (NumVertices==1671) && (primCount==2876))
#define t_anarchist_head ((Stride==32) && (NumVertices==1925) && (primCount==3314))
#define t_anarchist_body ((Stride==32) && (NumVertices==3881) && (primCount==6774))

#define t_anarchist t_anarchist_legs || t_anarchist_head || t_anarchist_body


//Professional
#define t_professional_legs ((Stride==32) && (NumVertices==2056) && (primCount==3436))
#define t_professional_body ((Stride==32) && (NumVertices==5116) && (primCount==8253))
#define t_professional_head1 ((Stride==32) && (NumVertices==927) && (primCount==1626))
#define t_professional_head2 ((Stride==32) && (NumVertices==646) && (primCount==1248))
#define t_professional_head3 ((Stride==32) && (NumVertices==1053) && (primCount==1968))
#define t_professional_head4 ((Stride==32) && (NumVertices==1197) && (primCount==2256))
#define t_professional_head5 ((Stride==32) && (NumVertices==924) && (primCount==1632))
#define t_professional_glasses1 ((Stride==32) && (NumVertices==266) && (primCount==348))
#define t_professional_glasses2 ((Stride==32) && (NumVertices==200) && (primCount==276))

#define t_professional t_professional_legs || t_professional_body || t_professional_head1 || t_professional_head2 || t_professional_head3 || t_professional_head4 || t_professional_head5 || t_professional_glasses1 || t_professional_glasses2


//Phoenix
#define t_phoenix_legs ((Stride==32) && (NumVertices==2599) && (primCount==4097))
#define t_phoenix_body ((Stride==32) && (NumVertices==2944) && (primCount==4916))
#define t_phoenix_head ((Stride==32) && (NumVertices==876) && (primCount==1630))

#define t_phoenix t_phoenix_legs || t_phoenix_body || t_phoenix_head  


//Balkan
#define t_balkan_legs1 ((Stride==32) && (NumVertices==1525) && (primCount==2525))
#define t_balkan_legs2 ((Stride==32) && (NumVertices==1273) && (primCount==2113))
#define t_balkan_body ((Stride==32) && (NumVertices==5215) && (primCount==8130))
#define t_balkan_head1 ((Stride==32) && (NumVertices==1214) && (primCount==2292))
#define t_balkan_head2 ((Stride==32) && (NumVertices==1215) && (primCount==2286))
#define t_balkan_head3 ((Stride==32) && (NumVertices==1291) && (primCount==2437))
#define t_balkan_head4 ((Stride==32) && (NumVertices==1345) && (primCount==2488))
#define t_balkan_head5 ((Stride==32) && (NumVertices==1624) && (primCount==2936))

#define t_balkan t_balkan_legs1 || t_balkan_legs2 || t_balkan_body || t_balkan_head1 || t_balkan_head2 || t_balkan_head3 || t_balkan_head4 || t_balkan_head5


//Pirate
#define t_pirate_legs1 ((Stride==32) && (NumVertices==934) && (primCount==1525))
#define t_pirate_legs2 ((Stride==32) && (NumVertices==991) && (primCount==1525))
#define t_pirate_body ((Stride==32) && (NumVertices==7900) && (primCount==9248))
#define t_pirate_head1 ((Stride==32) && (NumVertices==1996) && (primCount==2906))
#define t_pirate_head2 ((Stride==32) && (NumVertices==1183) && (primCount==2045))
#define t_pirate_head3 ((Stride==32) && (NumVertices==1380) && (primCount==2493))
#define t_pirate_head4 ((Stride==32) && (NumVertices==1314) && (primCount==2367))

#define t_pirate t_pirate_legs1 || t_pirate_legs2 || t_pirate_body || t_pirate_head1 || t_pirate_head2 || t_pirate_head3 || t_pirate_head4


// models\weapons\ct_arms.mdl
#define ct_arms_0 ((Stride=32) && (NumVertices==139) && (primCount==186))
#define ct_arms_1 ((Stride=32) && (NumVertices==296) && (primCount==167))
#define ct_arms_2 ((Stride=32) && (NumVertices==227) && (primCount==127))
#define ct_arms_3 ((Stride=32) && (NumVertices==345) && (primCount==694))
#define ct_arms_4 ((Stride=32) && (NumVertices==12) && (primCount==10))
#define ct_arms_5 ((Stride=32) && (NumVertices==1751) && (primCount==2482))
#define ct_arms_6 ((Stride=32) && (NumVertices==14) && (primCount==10))
#define ct_arms_7 ((Stride=32) && (NumVertices==492) && (primCount==892))
#define ct_arms_8 ((Stride=32) && (NumVertices==335) && (primCount==523))
#define ct_arms_9 ((Stride=32) && (NumVertices==108) && (primCount==104))
#define ct_arms_10 ((Stride=32) && (NumVertices==30) && (primCount==28))
#define ct_arms_11 ((Stride=32) && (NumVertices==8772) && (primCount==16398))
#define ct_arms ct_arms_0 || ct_arms_1 || ct_arms_2 || ct_arms_3 || ct_arms_4 || ct_arms_5 || ct_arms_6 || ct_arms_7 || ct_arms_8 || ct_arms_9 || ct_arms_10 || ct_arms_11

//------------------------------First Person arms-------------------------------------------
//models\weapons\t_arms.mdl
#define t_arms_0 ((Stride=32) && (NumVertices==8772) && (primCount==16398))
#define t_arms_1 ((Stride=32) && (NumVertices==6952) && (primCount==12834))
#define t_arms_b t_arms_0 || t_arms_1

//models\weapons\t_arms_anarchist.mdl
#define t_arms_anarchist_0 ((Stride=32) && (NumVertices==6952) && (primCount==12834))
#define t_arms_anarchist_1 ((Stride=32) && (NumVertices==7948) && (primCount==14512))
#define t_arms_anarchist t_arms_anarchist_0 || t_arms_anarchist_1 || t_arms_b || t_arms_anarchist
//models\weapons\t_arms_balkan.mdl
#define t_arms_balkan_0 ((Stride=32) && (NumVertices==10644) && (primCount==19878))

//models\weapons\t_arms_leet.mdl
#define t_arms_leet_0 ((Stride=32) && (NumVertices==6952) && (primCount==12834))

//models\weapons\t_arms_phoenix.mdl
#define t_arms_phoenix_0 ((Stride=32) && (NumVertices==6952) && (primCount==12834))
#define t_arms_phoenix_1 ((Stride=32) && (NumVertices==6954) && (primCount==12834))
#define t_arms_phoenix t_arms_phoenix_0 || t_arms_phoenix_1

//models\weapons\t_arms_pirate.mdl
#define t_arms_pirate_0 ((Stride=32) && (NumVertices==6954) && (primCount==12834))
#define t_arms_pirate_1 ((Stride=32) && (NumVertices==6414) && (primCount==11986))
#define t_arms_pirate_2 ((Stride=32) && (NumVertices==1784) && (primCount==1342))
#define t_arms_pirate t_arms_pirate_0 || t_arms_pirate_1 || t_arms_pirate_2

//models\weapons\t_arms_professional.mdl
#define t_arms_professional_0 ((Stride=32) && (NumVertices==9078) && (primCount==16780))

//models\weapons\t_arms_separatist.mdl
#define t_arms_separatist_0 ((Stride=32) && (NumVertices==9078) && (primCount==16780))
#define t_arms_separatist_1 ((Stride=32) && (NumVertices==9250) && (primCount==17036))
#define t_arms_separatist t_arms_separatist_0 || t_arms_separatist_1
#define t_arms t_arms_b || t_arms_anarchist || t_arms_balkan_0 || t_arms_leet_0 || t_arms_phoenix || t_arms_pirate || t_arms_professional_0 || t_arms_separatist

#define arms t_arms || ct_arms
#define ct_all ct_sas || ct_gign || ct_swat || ct_idf || ct_fbi || ct_fbi || ct_arms 
#define t_all t_elite || t_separatist || t_anarchist || t_professional || t_phoenix || t_balkan || t_pirate

#define aimkey (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
#define ENEMY (stride == 40 && sizeOfData == 1660 && NumVertices != 4059) //models & weapons
#define PLAYER ((stride == 40 && NumVertices == 358 && primCount == 572)||(stride == 40 && NumVertices == 3519 && primCount == 5908)) //player


#endif