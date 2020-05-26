
// MFC_Experiments.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "CoinFlipDlg.h"
#include "QueueingDlg.h"
#include "InventoryDlg.h"
#include "InventoryResultListDlg.h"


// CMFC_ExperimentsApp:
// See MFC_Experiments.cpp for the implementation of this class
//

class CMFC_ExperimentsApp : public CWinApp
{
public:
	CMFC_ExperimentsApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CMFC_ExperimentsApp theApp;