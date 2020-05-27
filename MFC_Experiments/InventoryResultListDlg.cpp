// InventoryResultListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Experiments.h"
#include "InventoryResultListDlg.h"
#include "afxdialogex.h"
#include "RandomStream.h"

// InventoryResultListDlg dialog


IMPLEMENT_DYNAMIC(InventoryResultListDlg, CDialogEx)

	InventoryResultListDlg::InventoryResultListDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(InventoryResultListDlg::IDD, pParent)
{
}

InventoryResultListDlg::~InventoryResultListDlg()
{
}

static std::vector<std::vector<CString>> result_columns=std::vector<std::vector<CString>>();

void InventoryResultListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, InventoryResultListTable);
	DWORD dwstyle = InventoryResultListTable.GetExtendedStyle();
	dwstyle |=LVS_EX_FULLROWSELECT;
	dwstyle |= LVS_EX_GRIDLINES;
	InventoryResultListTable.SetExtendedStyle(dwstyle);
	InventoryResultListTable.InsertColumn(0,_T("ID"),LVCFMT_LEFT,40);
	InventoryResultListTable.InsertColumn(1,_T("Simulation Length"),LVCFMT_LEFT,140);
	InventoryResultListTable.InsertColumn(2,_T("Policy"),LVCFMT_LEFT,100);
	InventoryResultListTable.InsertColumn(3,_T("Avg holding cost"),LVCFMT_LEFT,160);
	InventoryResultListTable.InsertColumn(4,_T("Avg ordering cost"),LVCFMT_LEFT,160);
	InventoryResultListTable.InsertColumn(5,_T("Avg shortage cost"),LVCFMT_LEFT,160);
	InventoryResultListTable.InsertColumn(6,_T("Avg total cost"),LVCFMT_LEFT,160);
	//	InventoryResultListTable.InsertColumn(7,_T("Proportion of backlog(time)"),LVCFMT_LEFT,160);
	//	InventoryResultListTable.InsertColumn(8,_T("Number of express orders"),LVCFMT_LEFT,160);
	//	InventoryResultListTable.InsertColumn(9,_T("Proportion of items discarded"),LVCFMT_LEFT,160);
}


BEGIN_MESSAGE_MAP(InventoryResultListDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &InventoryResultListDlg::OnLvnItemchangedList1)
END_MESSAGE_MAP()


// InventoryResultListDlg message handlers


void InventoryResultListDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
