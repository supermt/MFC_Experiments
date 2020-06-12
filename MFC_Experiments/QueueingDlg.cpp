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
	QueueingResultList.InsertColumn(0,_T("Simulation"),LVCFMT_CENTER,105);

	QueueingResultList.InsertColumn(1,_T("Average Delay"),LVCFMT_CENTER,105);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(2,_T("Average Number"),LVCFMT_CENTER,105);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(3,_T("The time-average number in the system"),LVCFMT_CENTER,105);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(4,_T("The average total time in the sysstem"),LVCFMT_CENTER,105);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(5,_T("The maximum queue length"),LVCFMT_CENTER,105);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(6,_T("The maximum delay in queue"),LVCFMT_CENTER,105);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(7,_T("The maximum time in system"),LVCFMT_CENTER,105);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(8,_T("The proportion of customers having a delay in excess"),LVCFMT_CENTER,105);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(9,_T("Service Utilization"),LVCFMT_CENTER,105);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(10,_T("Time simulation"),LVCFMT_CENTER,105);
	result_lines.push_back(std::vector<float>());
	QueueingResultList.InsertColumn(11,_T("Balk"),LVCFMT_CENTER,105);
	result_lines.push_back(std::vector<float>());

	DDX_Control(pDX, Radio_Fixed_Customer, fixed_customer_or_not);
	DDX_Control(pDX, Radio_Fixed_Length,fixed_length_or_not);
	DDX_Control(pDX, IDC_TIME_RANGE, limit_open_time_or_not);
	DDX_Control(pDX, IDC_RADIO_QUEUE_LIMITED,limit_capacity_or_not);

	DDX_Control(pDX, Button_RUN, RunButton);

//	RunButton.EnableWindow(0);
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
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("Fixed Customer %d"), result->seed); break;
	case 2:		
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("Fixed Simulation Length %d"), result->seed);break;
	case 3:		
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("Limited Serving Time %d"), result->seed);break;
	case 4:		
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("Limited Capacity %d"), result->seed);break;
	default:
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("Other %d"), result->seed);
		break;
	}

	int index = list_controller->InsertItem(row_count,buffer);
	memset(buffer,0,BUFFER_SIZE);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->avg_delay);
	result_lines[0].push_back(result->avg_delay);
	list_controller->SetItemText(index, 1,buffer);
	memset(buffer,0,BUFFER_SIZE);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->avg_number);
	result_lines[1].push_back(result->avg_number);
	list_controller->SetItemText(index, 2,buffer);
	memset(buffer,0,BUFFER_SIZE);

	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->avg_time_num_in_sys);
	result_lines[2].push_back(result->avg_time_num_in_sys);
	list_controller->SetItemText(index, 3,buffer);
	memset(buffer,0,BUFFER_SIZE);
	
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->avg_total_time_in_sys);
	list_controller->SetItemText(index, 4,buffer);
	result_lines[3].push_back(result->avg_total_time_in_sys);
	memset(buffer,0,BUFFER_SIZE);

	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->max_queue_length);
	result_lines[4].push_back(result->max_queue_length);
	list_controller->SetItemText(index, 5,buffer);
	memset(buffer,0,BUFFER_SIZE);


	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->max_delay_in_queue);
	result_lines[5].push_back(result->max_delay_in_queue);
	list_controller->SetItemText(index, 6,buffer);
	memset(buffer,0,BUFFER_SIZE);

	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->max_time_int_sys);
	list_controller->SetItemText(index, 7,buffer);
	result_lines[6].push_back(result->max_time_int_sys);
	memset(buffer,0,BUFFER_SIZE);
	
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->prop_cus_delay_excess);
	list_controller->SetItemText(index, 8,buffer);
	result_lines[7].push_back(result->prop_cus_delay_excess);
	memset(buffer,0,BUFFER_SIZE);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->server_util);
	result_lines[8].push_back(result->server_util);
	list_controller->SetItemText(index, 9,buffer);
	memset(buffer,0,BUFFER_SIZE);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->sim_tim);
	result_lines[9].push_back(result->sim_tim);
	list_controller->SetItemText(index, 10,buffer);
	memset(buffer,0,BUFFER_SIZE);
	StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%20.2f"), result->balk);
	result_lines[10].push_back(result->balk);
	list_controller->SetItemText(index, 11,buffer);
}

void QueueingDlg::run_simulation_once(QueueingClass* result_row,int seed,int game_type,float avg_arrive,float avg_service,float third_para){
	
	ResultHandler result;
	result.seed = seed;
	result_row->seed = seed;
	result_row->initialize(seed);
	result = result_row->bootstrap(avg_arrive,avg_service,third_para);
	add_result_line(&result,&QueueingResultList,row_count,game_type);
	row_count++;
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

	for (auto vec : result_lines){
		vec.clear();
	}

//	RunButton.EnableWindow(0);

	CString replication_cs;
	CString avg_arrive_cs;
	CString avg_serve_cs;
	CString third_para_cs;
	input_replication_times.GetWindowText(replication_cs);
	input_avg_interarrive.GetWindowText(avg_arrive_cs);
	input_avg_service_time.GetWindowText(avg_serve_cs);
	
	QueueingClass* result_row= nullptr;
	int game_type = 0;

	if (fixed_customer_or_not.GetCheck()){
		game_type = 1;
		result_row = new FixedCustomerQueueing();
		input_customers.GetWindowText(third_para_cs);
	}
	if (fixed_length_or_not.GetCheck()){
		game_type = 2;
		result_row = new FixedLengthQueueing();
		input_simulation_length.GetWindowText(third_para_cs);
	}
	if (limit_open_time_or_not.GetCheck()){
		game_type = 3;
		result_row = new LimitOpenTimeQueueing();
		CString tmp;
		input_open_time.GetWindowText(tmp);
		result_row->m_open_time = _ttoi(tmp);
		input_close_time.GetWindowText(tmp);
		result_row->m_close_time = _ttoi(tmp);
	}
	if (limit_capacity_or_not.GetCheck()){
		game_type =4;
		result_row = new LimitCapacityQueueing();
	}
	CString temp;
	input_customers.GetWindowText(temp);
	result_row->num_delays_required=_ttoi(temp);
	input_delay_limit.GetWindowText(temp);
	result_row->excess_delay=_ttoi(temp);
	input_queue_capacity.GetWindowText(temp);
	result_row->queue_capacity=_ttoi(temp);
	input_simulation_length.GetWindowText(temp);
	result_row->sim_time_required = _ttoi(temp);


	int replication = _ttoi(replication_cs);
	float avg_arrive = _ttof(avg_arrive_cs);
	float avg_serve = _ttof(avg_serve_cs);
	float third_para = _ttof(third_para_cs);
	for (int run_seed : seed_list){
		run_simulation_once(result_row,run_seed, game_type,avg_arrive,avg_serve,third_para);
	}
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

	//	RunButton.EnableWindow(1);

}


void QueueingDlg::OnBnClickedClearResult()
{
	// TODO: Add your control notification handler code here

	QueueingResultList.DeleteAllItems();
	row_count=0;
	for (auto vec : result_lines){
		vec.clear();
	}
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
