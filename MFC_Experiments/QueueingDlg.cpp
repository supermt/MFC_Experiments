// QueueingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Experiments.h"
#include "QueueingDlg.h"
#include "afxdialogex.h"
#include "strsafe.h"


// QueueingDlg dialog

IMPLEMENT_DYNAMIC(QueueingDlg, CDialogEx)

	QueueingDlg::QueueingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(QueueingDlg::IDD, pParent)
{
	row_count = 0;
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

	int column_width = 90;
	// initial the data in result list
	QueueingResultList.InsertColumn(0,_T("ID"),LVCFMT_LEFT,30);
	QueueingResultList.InsertColumn(1,_T("Average Delay"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(2,_T("Time-Avg Number in Queue"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(3,_T("Server Util"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(4,_T("Time-Avg Number in System"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(5,_T("Average Time in System"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(6,_T("Max Queue Length"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(7,_T("Max Time in Queue"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(8,_T("Max Time in System"),LVCFMT_LEFT,column_width);
	QueueingResultList.InsertColumn(9,_T("Proporation of Customers over 1 mins"),LVCFMT_LEFT,column_width);
	//QueueingResultList.InsertColumn(9,_T("The ma"),LVCFMT_LEFT,column_width);
	//	QueueingResultList.InsertColumn(10,_T("Balk"),LVCFMT_LEFT,column_width);

	DDX_Control(pDX, Radio_Fixed_Customer, fixed_customer_or_not);
	DDX_Control(pDX, Radio_Fixed_Length,fixed_length_or_not);

	DDX_Control(pDX, Button_RUN, RunButton);
}


BEGIN_MESSAGE_MAP(QueueingDlg, CDialogEx)
	ON_BN_CLICKED(Button_RUN, &QueueingDlg::OnBnClickedRun)
	ON_BN_CLICKED(Button_Clear_Result, &QueueingDlg::OnBnClickedClearResult)
	ON_BN_CLICKED(Radio_Fixed_Customer, &QueueingDlg::OnBnClickedFixedCustomer)
	ON_BN_CLICKED(Radio_Fixed_Length, &QueueingDlg::OnBnClickedFixedLength)
END_MESSAGE_MAP()


// QueueingDlg message handlers


void QueueingDlg::OnBnClickedCheck1()
{
	// TODO: Add your control notification handler code here
}

/*
if (fixed_customer_or_not.GetCheck() == 1){
// skip 
return ;
}

*/

inline void add_result_line(ResultHandler *result,CListCtrl* list_controller,int row_count){
	TCHAR buffer[24];
	memset(buffer,0,24);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), row_count);
	int index = list_controller->InsertItem(row_count,buffer);
	memset(buffer,0,24);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->avg_delay);
	list_controller->SetItemText(index, 1,buffer);
	memset(buffer,0,24);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->time_avg_number_in_q);
	list_controller->SetItemText(index, 2,buffer);
	memset(buffer,0,24);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->server_util);
	list_controller->SetItemText(index, 3,buffer);
	memset(buffer,0,24);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->time_avg_number_in_system);
	list_controller->SetItemText(index, 4,buffer);
	memset(buffer,0,24);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->avg_time_in_system);
	list_controller->SetItemText(index, 5,buffer);
	memset(buffer,0,24);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->queue_length_max);
	list_controller->SetItemText(index, 6,buffer);
	memset(buffer,0,24);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->max_delay_in_queue);
	list_controller->SetItemText(index, 7,buffer);
	memset(buffer,0,24);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->max_time_in_system);
	list_controller->SetItemText(index, 8,buffer);
	memset(buffer,0,24);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->propo_customers_over_1min);
	list_controller->SetItemText(index, 9,buffer);
	/*int index = list_controller->InsertItem(0,buffer);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), 1.0f);
	list_controller->SetItemText(index, 1,buffer);
	*/
}

void QueueingDlg::OnBnClickedRun()
{
	QueueingClass* result_row= nullptr;
	RunButton.EnableWindow(0);
	ResultHandler result;
	if (fixed_customer_or_not.GetCheck()){
		result_row = new FixedCustomerQueueing();
		result = result_row->bootstrap(1.0,0.5,1000);
	}

	if (fixed_length_or_not.GetCheck()){
		result_row = new FixedLengthQueueing();
		result = result_row->bootstrap(1.0,0.5,480.0);
	}

	add_result_line(&result,&QueueingResultList,row_count);
	row_count++;
	RunButton.EnableWindow(1);

	if (!result_row){
		return ;
	}else{
		free(result_row);
		return ;
	}

}


void QueueingDlg::OnBnClickedClearResult()
{
	// TODO: Add your control notification handler code here
}



void QueueingDlg::OnBnClickedFixedCustomer()
{
	// TODO: Add your control notification handler code here
	if (fixed_customer_or_not.GetCheck()){
		// skip 
		fixed_customer_or_not.SetWindowText(L"Fixed Customers triggered");
		fixed_length_or_not.SetWindowText(L"Fixed Length");

		return ;
	}
}


void QueueingDlg::OnBnClickedFixedLength()
{
	// TODO: Add your control notification handler code here
	if (fixed_length_or_not.GetCheck()){
		// skip 
		fixed_length_or_not.SetWindowText(L"Fixed Length triggered");
		fixed_customer_or_not.SetWindowText(L"Fixed Customers");
		return ;
	}
}
