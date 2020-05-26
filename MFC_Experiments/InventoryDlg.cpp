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

}

InventoryDlg::~InventoryDlg()
{
}

void InventoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(InventoryDlg, CDialogEx)
END_MESSAGE_MAP()


// InventoryDlg message handlers
