#ifndef DARK_H
#define DARK_H

#include <windows.h>
#include <uxtheme.h>
#include <dwmapi.h>

static COLORREF BG_COLOR_DARK = 0x202020;
static COLORREF TEXT_COLOR_DARK = 0xe0e0e0;
static HBRUSH BG_BRUSH_DARK = CreateSolidBrush(BG_COLOR_DARK);

static HBRUSH MENUBAR_BG_BRUSH_DARK = CreateSolidBrush(COLORREF(0x2b2b2b));
static HBRUSH MENUBAR_BG_BRUSH_HOT_DARK = CreateSolidBrush(COLORREF(0x3e3e3e));

static COLORREF CONTROL_BG_COLOR_DARK = COLORREF(0x333333);
static HBRUSH CONTROL_BG_BRUSH_DARK = CreateSolidBrush(CONTROL_BG_COLOR_DARK);

static COLORREF CONTROL_BG_COLOR_HOT_DARK = COLORREF(0x454545);
static HBRUSH CONTROL_BG_BRUSH_HOT_DARK = CreateSolidBrush(CONTROL_BG_COLOR_HOT_DARK);

static COLORREF CONTROL_BG_COLOR_SELECTED_DARK = COLORREF(0x666666);
static HBRUSH CONTROL_BG_BRUSH_SELECTED_DARK = CreateSolidBrush(CONTROL_BG_COLOR_SELECTED_DARK);

static COLORREF CONTROL_BORDER_COLOR_DARK = COLORREF(0x9B9B9B);
static HBRUSH CONTROL_BORDER_BRUSH_DARK = CreateSolidBrush(CONTROL_BORDER_COLOR_DARK);

static HBRUSH SEPARATOR_BRUSH_DARK = CreateSolidBrush(COLORREF(0x646464));

static HBRUSH TAB_BORDER_BRUSH_DARK = CreateSolidBrush(COLORREF(0x333333));
static HBRUSH TAB_SELECTED_BG_BRUSH_DARK = CreateSolidBrush(COLORREF(0x333333));
static HBRUSH TAB_SELECTED_HILITE_BRUSH = CreateSolidBrush(COLORREF(0xFF9933));

static HBRUSH DARK_TOOLBAR_BUTTON_BORDER_BRUSH = CreateSolidBrush(COLORREF(0x636363));
static HBRUSH DARK_TOOLBAR_BUTTON_BG_BRUSH = CreateSolidBrush(COLORREF(0x424242));

static COLORREF DARK_TOOLBAR_BUTTON_ROLLOVER_BG_COLOR = COLORREF(0x434343); // 0x2b2b2b);
static HBRUSH DARK_TOOLBAR_BUTTON_ROLLOVER_BG_BRUSH = CreateSolidBrush(DARK_TOOLBAR_BUTTON_ROLLOVER_BG_COLOR);

// for testing stuff
//#define DBGI(x) {char dbg[32];sprintf(dbg,"%ld",x);OutputDebugStringA(dbg);}
static HBRUSH BRUSH_YELLOW = CreateSolidBrush(COLORREF(0x00ffff));

#define STATUSBAR_SUBCLASS_ID 1

// window messages related to menu bar drawing
#define WM_UAHDESTROYWINDOW    0x0090	// handled by DefWindowProc
#define WM_UAHDRAWMENU         0x0091	// lParam is UAHMENU
#define WM_UAHDRAWMENUITEM     0x0092	// lParam is UAHDRAWMENUITEM
#define WM_UAHINITMENU         0x0093	// handled by DefWindowProc
#define WM_UAHMEASUREMENUITEM  0x0094	// lParam is UAHMEASUREMENUITEM
#define WM_UAHNCPAINTMENUPOPUP 0x0095	// handled by DefWindowProc

// describes the sizes of the menu bar or menu item
typedef union tagUAHMENUITEMMETRICS
{
	// cx appears to be 14 / 0xE less than rcItem's width!
	// cy 0x14 seems stable, i wonder if it is 4 less than rcItem's height which is always 24 atm
	struct
	{
		DWORD cx;
		DWORD cy;
	} rgsizeBar[2];
	struct
	{
		DWORD cx;
		DWORD cy;
	} rgsizePopup[4];
} UAHMENUITEMMETRICS;

// not really used in our case but part of the other structures
typedef struct tagUAHMENUPOPUPMETRICS
{
	DWORD rgcx[4];
	DWORD fUpdateMaxWidths : 2; // from kernel symbols, padded to full dword
} UAHMENUPOPUPMETRICS;

// hmenu is the main window menu; hdc is the context to draw in
typedef struct tagUAHMENU
{
	HMENU hmenu;
	HDC hdc;
	DWORD dwFlags; // no idea what these mean, in my testing it's either 0x00000a00 or sometimes 0x00000a10
} UAHMENU;

// menu items are always referred to by iPosition here
typedef struct tagUAHMENUITEM
{
	int iPosition; // 0-based position of menu item in menubar
	UAHMENUITEMMETRICS umim;
	UAHMENUPOPUPMETRICS umpm;
} UAHMENUITEM;

// the DRAWITEMSTRUCT contains the states of the menu items, as well as
// the position index of the item in the menu, which is duplicated in
// the UAHMENUITEM's iPosition as well
typedef struct UAHDRAWMENUITEM
{
	DRAWITEMSTRUCT dis; // itemID looks uninitialized
	UAHMENU um;
	UAHMENUITEM umi;
} UAHDRAWMENUITEM;

// the MEASUREITEMSTRUCT is intended to be filled with the size of the item
// height appears to be ignored, but width can be modified
typedef struct tagUAHMEASUREMENUITEM
{
	MEASUREITEMSTRUCT mis;
	UAHMENU um;
	UAHMENUITEM umi;
} UAHMEASUREMENUITEM;

enum PreferredAppMode
{
	Default,
	AllowDark,
	ForceDark,
	ForceLight,
	Max
};

using fnOpenNcThemeData = HTHEME(WINAPI*)(HWND hWnd, LPCWSTR pszClassList);
using fnSetPreferredAppMode = PreferredAppMode(WINAPI*)(PreferredAppMode appMode);
using fnFlushMenuThemes = void (WINAPI*)();

void FixDarkScrollBar();
void InitDarkMode();

#endif
