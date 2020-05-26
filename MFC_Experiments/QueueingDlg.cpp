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
	
	DDX_Control(pDX, IDC_LIST5, QueueingResultList);
	DWORD dwstyle = QueueingResultList.GetExtendedStyle();
	dwstyle |=LVS_EX_FULLROWSELECT;
	dwstyle |= LVS_EX_GRIDLINES;
	QueueingResultList.SetExtendedStyle(dwstyle);

	int column_width = 60;
	// initial the data in result list
	QueueingResultList.InsertColumn(0,_T("Simulation"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(1,_T("Average"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(2,_T("Average"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(3,_T("The time"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(4,_T("The Aver"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(5,_T("The ma"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(6,_T("The ma"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(7,_T("The ma"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(8,_T("The ma"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(9,_T("The ma"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(10,_T("Balk"),LVCFMT_LEFT,column_width);

}


BEGIN_MESSAGE_MAP(QueueingDlg, CDialogEx)
END_MESSAGE_MAP()


// QueueingDlg message handlers


void QueueingDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
}
