#pragma once
#include "afxcmn.h"
#include "Inv_handler.h"
#include <vector>

// InventoryResultListDlg dialog

class InventoryResultListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InventoryResultListDlg)

public:
	InventoryResultListDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~InventoryResultListDlg();

// Dialog Data
	enum { IDD = InventoryResultList };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl InventoryResultListTable;

	static std::vector<std::vector<CString>*> result_columns;
	/*
	the format is like:
	ID, Simulation time, Policy Column, avgtime....

	*/
	
};
