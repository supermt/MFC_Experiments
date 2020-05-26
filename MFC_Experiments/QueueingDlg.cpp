// QueueingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Experiments.h"
#include "QueueingDlg.h"
#include "afxdialogex.h"


// QueueingDlg dialog

IMPLEMENT_DYNAMIC(QueueingDlg, CDialogEx)

QueueingDlg::QueueingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(QueueingDlg::IDD, pParent)
{

}

QueueingDlg::~QueueingDlg()
{
}

void QueueingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(QueueingDlg, CDialogEx)
END_MESSAGE_MAP()


// QueueingDlg message handlers
