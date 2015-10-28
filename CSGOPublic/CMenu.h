#pragma once

#include <unordered_map>
#include "stdafx.h"

class CVarHead;
class CMenuPanel;
class CMenu;
class CVarManager;

//=======================================
//=======================================
//Logic for menu
//=======================================
//=======================================
//Specific Vars -> this is where I need a hash.

typedef union
{
	float val_float;
	UINT val_int;
	bool val_bool;
} var_type;

extern var_type *vVars;

//SubMenu Vars
class CVarHead
{
public:
	CVarHead::CVarHead(std::string inname)
	{
		name = inname;
	}
	void AddSubVar(std::string name, var_type v);

private:
	int iCountVars;
	std::string name;
};
//Panel Vars
class CVarManager
{
public:

	CVarManager::CVarManager(std::string sSubName, CVarHead *pFirstSub)
	{
		pFirst = new CVarHead(sSubName);
	}

	int iCountSubs;
	bool bMainSub;
	CVarHead *pFirst; //Aimbot, ESP, Utility, DirectX, Extras
};

//==========================================
//==========================================
//Drawing
//==========================================
//==========================================

class CMenuPanel
{
public:
	CMenuPanel::CMenuPanel(std::string inputName, int inx, int iny, int inh, int inw, unsigned int inFont)
	{
		name = inputName;
		startx = inx;
		starty = iny;
		startwidth = inw;
		startheight = inh;
		Font = inFont;
	}

	void Draw();
	void AddSubMenu(CMenuPanel cAddMenu);

private:
	std::string name;
	int startx;
	int starty;
	int startwidth;
	int startheight;
	unsigned int Font;
};

class CMenu
{
public:
	CMenuPanel *cMenuPanelList;
};

extern CMenu cmMenu;
extern CMenu cmColorPicker;