// CoinFlipDlg.cpp : implementation file


#include "stdafx.h"
#include "MFC_Experiments.h"
#include "CoinFlipDlg.h"
#include "afxdialogex.h"


// CoinFlipDlg dialog

IMPLEMENT_DYNAMIC(CoinFlipDlg, CDialogEx)

CoinFlipDlg::CoinFlipDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CoinFlipDlg::IDD, pParent)
{

}

CoinFlipDlg::~CoinFlipDlg()
{
}

void CoinFlipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CoinFlipDlg, CDialogEx)
END_MESSAGE_MAP()


// CoinFlipDlg message handlers
