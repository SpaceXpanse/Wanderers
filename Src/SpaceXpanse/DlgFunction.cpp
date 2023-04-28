// Copyright (c) Martin Schweiger
// Licensed under the MIT License

// ======================================================================
// Custom function selection dialog
// ======================================================================

#define STRICT 1

#include "DlgFunction.h"
#include "SpaceXpanse.h"
#include "Resource.h"
#include "Resource2.h"

extern SpaceXpanse *g_pSpaceXpanse;
extern HELPCONTEXT DefHelpContext;

// ======================================================================

DlgFunction::DlgFunction (HINSTANCE hInstance, HWND hParent, void *context)
: DialogWin (hInstance, hParent, IDD_CUSTOMCMD, 0, 0, context)
{
}

// ======================================================================

void DlgFunction::ScanFunctions (HWND hDlg)
{
	DWORD i, idx;
	SendDlgItemMessage (hDlg, IDC_CUSTOMCMD_LIST, LB_RESETCONTENT, 0, 0);
	for (i = 0; i < g_pSpaceXpanse->ncustomcmd; i++)
		idx = SendDlgItemMessage (hDlg, IDC_CUSTOMCMD_LIST, LB_ADDSTRING, 0, (LPARAM)g_pSpaceXpanse->customcmd[i].label);
	if (i) SendDlgItemMessage (hDlg, IDC_CUSTOMCMD_LIST, LB_SETCURSEL, 0, 0);
}

// ======================================================================

void DlgFunction::RunFunction (HWND hDlg)
{
	int idx = SendDlgItemMessage (hDlg, IDC_CUSTOMCMD_LIST, LB_GETCURSEL, 0, 0);
	if (idx != LB_ERR) g_pSpaceXpanse->customcmd[idx].func (g_pSpaceXpanse->customcmd[idx].context);
}

// ======================================================================

void DlgFunction::ShowDescription (HWND hDlg)
{
	int idx = SendDlgItemMessage (hDlg, IDC_CUSTOMCMD_LIST, LB_GETCURSEL, 0, 0);
	if (idx != LB_ERR)
		SetWindowText (GetDlgItem (hDlg, IDC_CUSTOMCMD_DESCR), g_pSpaceXpanse->customcmd[idx].desc);
}

// ======================================================================

BOOL DlgFunction::OnInitDialog (HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	ScanFunctions (hDlg);
	ShowDescription (hDlg);
	return TRUE;
}

// ======================================================================

BOOL DlgFunction::OnCommand (HWND hDlg, WORD id, WORD code, HWND hControl)
{
	switch (id) {
	case IDHELP:
		DefHelpContext.topic = "/customcmd.htm";
		g_pSpaceXpanse->OpenHelp (&DefHelpContext);
		return TRUE;
	case IDOK:
		RunFunction (hDlg);
		g_pSpaceXpanse->CloseDialog (hDlg);
		return TRUE;
	case IDC_CUSTOMCMD_LIST:
		switch (code) {
		case LBN_DBLCLK:
			RunFunction (hDlg);
			g_pSpaceXpanse->CloseDialog (hDlg);
			return TRUE;
		case LBN_SELCHANGE:
			ShowDescription (hDlg);
			return TRUE;
		}
		break;
	}
	return DialogWin::OnCommand (hDlg, id, code, hControl);
}
