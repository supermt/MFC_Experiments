#pragma once
#include "afxcmn.h"

#include "InventoryResultListDlg.h"

// InventoryDlg dialog

class InventoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InventoryDlg)

public:
	InventoryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~InventoryDlg();

// Dialog Data
	enum { IDD = InventoryTag };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl PolicyList;
	afx_msg void OnBnClickedButton4();
};
