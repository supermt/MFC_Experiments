// InventoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Experiments.h"
#include "InventoryDlg.h"
#include "afxdialogex.h"



// InventoryDlg dialog

IMPLEMENT_DYNAMIC(InventoryDlg, CDialogEx)

	InventoryDlg::InventoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(InventoryDlg::IDD, pParent)
{

	for (auto seed : default_seeds){
		RandomGenerator::zrng.push_back(seed);
	}
}

InventoryDlg::~InventoryDlg()
{	

	for (auto seed : default_seeds){
		RandomGenerator::zrng.push_back(seed);
	}
}

void InventoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, PolicyList);
	DWORD dwstyle = PolicyList.GetExtendedStyle();
	dwstyle |=LVS_EX_FULLROWSELECT;
	dwstyle |= LVS_EX_GRIDLINES;
	PolicyList.SetExtendedStyle(dwstyle);
	PolicyList.InsertColumn(0,_T("No."),LVCFMT_LEFT,40);
	PolicyList.InsertColumn(1,_T("Policy"),LVCFMT_LEFT,160);
}


BEGIN_MESSAGE_MAP(InventoryDlg, CDialogEx)
	ON_BN_CLICKED(INV_POP_RESULT_LIST, &InventoryDlg::OnBnClickedPopResult)
END_MESSAGE_MAP()


// InventoryDlg message handlers


void InventoryDlg::OnBnClickedPopResult()
{
	// TODO: Add your control notification handler code here
	InventoryResultListDlg newTag;
	newTag.DoModal();
}
