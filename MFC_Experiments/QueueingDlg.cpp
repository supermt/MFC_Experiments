// QueueingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Experiments.h"
#include "QueueingDlg.h"
#include "RandomStream.h"
#include "afxdialogex.h"
#include "strsafe.h"
#include <numeric>


#define BUFFER_SIZE 40

// QueueingDlg dialog

IMPLEMENT_DYNAMIC(QueueingDlg, CDialogEx)

	QueueingDlg::QueueingDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(QueueingDlg::IDD, pParent)
	, Radio_origin(0)
{
	row_count = 0;
	seed_list = std::vector<int>();
	
	for (auto seed : default_seeds){
		RandomGenerator::zrng.push_back(seed);
	}
	result_lines = std::vector<std::vector<float>>();
}

QueueingDlg::~QueueingDlg()
{	
}

void QueueingDlg::SetRepText(){
	CString input_rep_cs;
	input_rep_cs.Format(_T("%d"),seed_list.size());
	input_replication_times.SetWindowText(input_rep_cs);
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
	QueueingResultList.InsertColumn(0,_T("ID"),LVCFMT_LEFT,60);
	QueueingResultList.InsertColumn(1,_T("Average Delay"),LVCFMT_LEFT,column_width);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(2,_T("Time-Avg Number in Queue"),LVCFMT_LEFT,column_width);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(3,_T("Server Util"),LVCFMT_LEFT,column_width);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(4,_T("Time-Avg Number in System"),LVCFMT_LEFT,column_width);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(5,_T("Average Time in System"),LVCFMT_LEFT,column_width);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(6,_T("Max Queue Length"),LVCFMT_LEFT,column_width);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(7,_T("Max Time in Queue"),LVCFMT_LEFT,column_width);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(8,_T("Max Time in System"),LVCFMT_LEFT,column_width);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(9,_T("Proporation of Customers over 1 mins"),LVCFMT_LEFT,column_width);
	result_lines.push_back(std::vector<float>());
	//QueueingResultList.InsertColumn(9,_T("The ma"),LVCFMT_LEFT,column_width);
	//	QueueingResultList.InsertColumn(10,_T("Balk"),LVCFMT_LEFT,column_width);

	DDX_Control(pDX, Radio_Fixed_Customer, fixed_customer_or_not);
	DDX_Control(pDX, Radio_Fixed_Length,fixed_length_or_not);

	DDX_Control(pDX, Button_RUN, RunButton);

	RunButton.EnableWindow(0);
	DDX_Control(pDX, IDC_LIST1, RandomSeedPool);
	DDX_Control(pDX, INPUT_Interarrive, input_avg_interarrive);
	DDX_Control(pDX, INPUT_time_service, input_avg_service_time);
	DDX_Control(pDX, INPUT_customers, input_customers);
	DDX_Control(pDX, INPUT_Simulation_length, input_simulation_length);
	DDX_Control(pDX, INPUT_Excess_Delay, input_delay_limit);
	DDX_Control(pDX, INPUT_Replication, input_replication_times);
	OnBnClickedResetStream();
	DDX_Control(pDX, INPUT_seed, input_seed);



	DDX_Control(pDX, INPUT_OPEN, input_open_time);
	DDX_Control(pDX, INPUT_Close, input_close_time);
	DDX_Control(pDX, INPUT_Queue_Capacity, input_queue_capacity);
	input_open_time.SetWindowText(_T("9"));
	input_close_time.SetWindowText(_T("17"));
	input_open_time.EnableWindow(0);
	input_close_time.EnableWindow(0);
	input_queue_capacity.SetWindowText(_T("1000"));
	input_queue_capacity.EnableWindow(0);
}


BEGIN_MESSAGE_MAP(QueueingDlg, CDialogEx)
	ON_BN_CLICKED(Button_RUN, &QueueingDlg::OnBnClickedRun)
	ON_BN_CLICKED(Button_Clear_Result, &QueueingDlg::OnBnClickedClearResult)
	ON_BN_CLICKED(Radio_Fixed_Customer, &QueueingDlg::OnBnClickedFixedCustomer)
	ON_BN_CLICKED(Radio_Fixed_Length, &QueueingDlg::OnBnClickedFixedLength)
	ON_BN_CLICKED(Button_Reset_Stream, &QueueingDlg::OnBnClickedResetStream)
	ON_BN_CLICKED(IDC_BUTTON1, &QueueingDlg::OnBnClickedLoadDefault)
	ON_BN_CLICKED(Clear_Seed_List, &QueueingDlg::OnBnClickedClearSeedList)
	ON_BN_CLICKED(Button_Delete_Selected_Seed, &QueueingDlg::OnBnClickedDeleteSelectedSeed)
	ON_BN_CLICKED(Button_Add_Seed, &QueueingDlg::OnBnClickedAddSeed)
	ON_BN_CLICKED(IDC_TIME_RANGE, &QueueingDlg::OnBnClickedTimeRange)
	ON_BN_CLICKED(IDC_RADIO_QUEUE_LIMITED, &QueueingDlg::OnBnClickedRadioQueueLimited)
END_MESSAGE_MAP()



void QueueingDlg::add_result_line(ResultHandler *result,CListCtrl* list_controller,int row_count,int game_type){
	TCHAR buffer[BUFFER_SIZE];
	memset(buffer,0,BUFFER_SIZE);
	switch (game_type)
	{
	case 1:
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("Fixed Customer %d"), row_count); break;
	case 2:		
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("Fixed Simulation Length %d"), row_count);break;
	case 3:		
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("Variable Serving Time %d"), row_count);break;
	case 4:		
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("Limited Delay Time %d"), row_count);break;
	default:
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("Other %d"), row_count);
		break;
	}

	int index = list_controller->InsertItem(row_count,buffer);
	memset(buffer,0,BUFFER_SIZE);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->avg_delay);
	result_lines[0].push_back(result->avg_delay);
	list_controller->SetItemText(index, 1,buffer);
	memset(buffer,0,BUFFER_SIZE);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->time_avg_number_in_q);
	result_lines[1].push_back(result->time_avg_number_in_q);
	list_controller->SetItemText(index, 2,buffer);
	memset(buffer,0,BUFFER_SIZE);

	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->server_util);
	result_lines[2].push_back(result->server_util);
	list_controller->SetItemText(index, 3,buffer);
	memset(buffer,0,BUFFER_SIZE);
	
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->time_avg_number_in_system);
	list_controller->SetItemText(index, 4,buffer);
	result_lines[3].push_back(result->time_avg_number_in_system);
	memset(buffer,0,BUFFER_SIZE);

	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->avg_time_in_system);
	result_lines[4].push_back(result->avg_time_in_system);
	list_controller->SetItemText(index, 5,buffer);
	memset(buffer,0,BUFFER_SIZE);


	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->queue_length_max);
	result_lines[5].push_back(result->queue_length_max);
	list_controller->SetItemText(index, 6,buffer);
	memset(buffer,0,BUFFER_SIZE);

	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->max_delay_in_queue);
	list_controller->SetItemText(index, 7,buffer);
	result_lines[6].push_back(result->max_delay_in_queue);
	memset(buffer,0,BUFFER_SIZE);
	
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->max_time_in_system);
	list_controller->SetItemText(index, 8,buffer);
	result_lines[7].push_back(result->max_time_in_system);
	memset(buffer,0,BUFFER_SIZE);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->propo_customers_over_1min);
	result_lines[8].push_back(result->propo_customers_over_1min);
	list_controller->SetItemText(index, 9,buffer);
}

void QueueingDlg::run_simulation_once(int seed,int game_type,float avg_arrive,float avg_service,float third_para){
	QueueingClass* result_row= nullptr;
	ResultHandler result;
	switch (game_type)
	{
	case 1:
		result_row = new FixedCustomerQueueing();
		break;
	case 2:
		result_row = new FixedLengthQueueing();
		break;
	case 3:
		result_row = new FixedCustomerQueueing();
		break;
	case 4:
		result_row = new FixedCustomerQueueing();
		break;
	default:
		result_row = new FixedCustomerQueueing();
		break;
	}
	result_row->random_seed = seed;
	result = result_row->bootstrap(avg_arrive,avg_service,third_para);
	/*	if (fixed_customer_or_not.GetCheck()){

	result = result_row->bootstrap(avg_arrive,avg_service,third_para);
	}

	if (fixed_length_or_not.GetCheck()){

	}
	*/
	add_result_line(&result,&QueueingResultList,row_count,game_type);
	row_count++;
	CString tmp;
	int index = QueueingResultList.InsertItem(row_count,_T(" "));

	int index_mean = QueueingResultList.InsertItem(row_count+1,_T("Mean of index"));
	int index_stdev = QueueingResultList.InsertItem(row_count+2,_T("Stdev of index"));

	for (int i = 0;i <result_lines.size(); i++){
		std::vector<float> v = result_lines[i];
		double sum = std::accumulate(v.begin(), v.end(), 0.0);
		double mean = sum / v.size();
		double sq_sum = std::inner_product(v.begin(), v.end(), v.begin(), 0.0);
		double stdev = std::sqrt(sq_sum / v.size() - mean * mean);
		tmp.Format(_T("%.2f"),mean);
		QueueingResultList.SetItemText(index_mean, i+1,tmp);
		tmp.Format(_T("%.2f"),stdev);
		QueueingResultList.SetItemText(index_stdev, i+1,tmp);

	}

	if (!result_row){
		return ;
	}else{
		free(result_row);
		return ;
	}
}

void QueueingDlg::OnBnClickedRun()
{
	// clear all seeds
	std::fill(RandomGenerator::zrng.begin(), RandomGenerator::zrng.begin(), 0);
	// get all seed from randompool
	CString seedText;
	seed_list.clear();
	for(int i = 0; i < RandomSeedPool.GetCount(); i++)
	{
		RandomSeedPool.GetText(i, seedText);
		// RandomGenerator::zrng.insert(RandomGenerator::zrng.begin(),0, _ttol(seedText));
		seed_list.push_back(_ttoi(seedText) %100 +1);
	}



	RunButton.EnableWindow(0);

	CString replication_cs;
	CString avg_arrive_cs;
	CString avg_serve_cs;
	CString third_para_cs;
	input_replication_times.GetWindowText(replication_cs);
	input_avg_interarrive.GetWindowText(avg_arrive_cs);
	input_avg_service_time.GetWindowText(avg_serve_cs);

	int game_type = 0;
	if (fixed_customer_or_not.GetCheck()){
		game_type = 1;
		input_customers.GetWindowText(third_para_cs);
	}
	if (fixed_length_or_not.GetCheck()){
		game_type = 2;
		input_simulation_length.GetWindowText(third_para_cs);
	}
	else{
		third_para_cs="1000";
	}


	int replication = _ttoi(replication_cs);
	float avg_arrive = _ttof(avg_arrive_cs);
	float avg_serve = _ttof(avg_serve_cs);
	float third_para = _ttof(third_para_cs);
	for (int run_seed : seed_list){
		run_simulation_once(run_seed, game_type,avg_arrive,avg_serve,third_para);
	}
	RunButton.EnableWindow(1);

}


void QueueingDlg::OnBnClickedClearResult()
{
	// TODO: Add your control notification handler code here

	QueueingResultList.DeleteAllItems();
	UpdateWindow();
}



void QueueingDlg::OnBnClickedFixedCustomer()
{
	input_open_time.EnableWindow(0);
	input_close_time.EnableWindow(0);
	input_queue_capacity.EnableWindow(0);
	RunButton.EnableWindow(1);
	// TODO: Add your control notification handler code here
	if (fixed_customer_or_not.GetCheck()){
		// skip 
		fixed_customer_or_not.SetWindowText(L"Fixed Customers triggered");
		fixed_length_or_not.SetWindowText(L"Fixed Length");
		input_customers.EnableWindow(1);
		input_simulation_length.EnableWindow(0);
		return ;
	}
}


void QueueingDlg::OnBnClickedFixedLength()
{	
	input_open_time.EnableWindow(0);
	input_close_time.EnableWindow(0);
	input_queue_capacity.EnableWindow(0);
	RunButton.EnableWindow(1);
	// TODO: Add your control notification handler code here
	if (fixed_length_or_not.GetCheck()){
		// skip 
		fixed_length_or_not.SetWindowText(L"Fixed Length triggered");
		fixed_customer_or_not.SetWindowText(L"Fixed Customers");
		input_customers.EnableWindow(0);
		input_simulation_length.EnableWindow(1);
		return ;

	}
}


void QueueingDlg::OnBnClickedResetStream()
{
	// load all default seeds;
	OnBnClickedLoadDefault();

	input_avg_interarrive.SetWindowText(TEXT("1.0"));
	input_avg_service_time.SetWindowText(_T("0.5"));
	input_customers.SetWindowText(_T("1000"));
	input_customers.EnableWindow(0);
	input_simulation_length.SetWindowText(_T("480.0"));
	input_simulation_length.EnableWindow(0);
	input_delay_limit.SetWindowText(_T("N/A"));
}


void QueueingDlg::OnBnClickedLoadDefault()
{
	OnBnClickedClearSeedList();
	/*
	TCHAR buffer[BUFFER_SIZE];

	// TODO: Add your control notification handler code here
	for (auto seed : default_seeds){
		memset(buffer,0,BUFFER_SIZE);
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), seed);
		RandomSeedPool.AddString(buffer);
		RandomGenerator::zrng.push_back(seed);
	}
	*/
}


void QueueingDlg::OnBnClickedClearSeedList()
{
	// TODO: Add your control notification handler code here
	RandomSeedPool.ResetContent();
	seed_list.clear();
	SetRepText();
	UpdateWindow();
}


void QueueingDlg::OnBnClickedDeleteSelectedSeed()
{
	// TODO: Add your control notification handler code here
	CListBox * pList1 = (CListBox *)GetDlgItem(IDC_LIST1);

	int nSel = pList1->GetCurSel();
	if (nSel != LB_ERR)
	{
		CString ItemSelected; 
		pList1->GetText(nSel, ItemSelected);
		pList1->DeleteString(nSel);
		seed_list.erase(seed_list.begin()+nSel);
	}
	SetRepText();
}


void QueueingDlg::OnBnClickedAddSeed()
{
	// TODO: Add your control notification handler code here
	CString newSeed;
	input_seed.GetWindowText(newSeed);
	if (newSeed.IsEmpty()) AfxMessageBox(_T("Empty Seed"));
	else {
		RandomSeedPool.AddString(newSeed);
		seed_list.push_back(_ttoi(newSeed) % 100 + 1);
		SetRepText();
	}
}


void QueueingDlg::OnBnClickedLimiting()
{
	// TODO: Add your control notification handler code here

}


void QueueingDlg::OnBnClickedTimeRange()
{
	// TODO: Add your control notification handler code here
	input_open_time.EnableWindow(1);
	input_close_time.EnableWindow(1);

	input_queue_capacity.EnableWindow(0);
	RunButton.EnableWindow(1);

}


void QueueingDlg::OnBnClickedRadioQueueLimited()
{
	// TODO: Add your control notification handler code here
	input_open_time.EnableWindow(0);
	input_close_time.EnableWindow(0);
	input_queue_capacity.EnableWindow(1);
	RunButton.EnableWindow(1);

}
