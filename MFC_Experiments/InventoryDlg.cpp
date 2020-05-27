// InventoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC_Experiments.h"
#include "InventoryDlg.h"
#include "afxdialogex.h"
#include "strsafe.h"
#define BUFFER_SIZE 40

// InventoryDlg dialog

IMPLEMENT_DYNAMIC(InventoryDlg, CDialogEx)

	InventoryDlg::InventoryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(InventoryDlg::IDD, pParent)
{

	for (auto seed : default_seeds){
		RandomGenerator::zrng.push_back(seed);
	}
	current_policy_list = std::vector<Policy>();
	current_policy_list.clear();
	policy_count = 0;
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
	DDX_Control(pDX, INV_POLICY_LIST, PolicyList);
	DWORD dwstyle = PolicyList.GetExtendedStyle();
	dwstyle |=LVS_EX_FULLROWSELECT;
	dwstyle |= LVS_EX_GRIDLINES;
	PolicyList.SetExtendedStyle(dwstyle);
	PolicyList.InsertColumn(0,_T("No."),LVCFMT_LEFT,40);
	PolicyList.InsertColumn(1,_T("min lag"),LVCFMT_LEFT,80);
	PolicyList.InsertColumn(1,_T("max lag"),LVCFMT_LEFT,80);
	DDX_Control(pDX, INV_LIST_SEED, RandomSeedPool);
	DDX_Control(pDX, INV_RUN_BTN, RunButton);

	RunButton.EnableWindow(0);
	DDX_Control(pDX, INV_INPUT_SEED, input_seed);
	DDX_Control(pDX, INV_INPUT_REPEAT, input_rep);
	DDX_Control(pDX, INV_INITAL_LEVEL, input_initial_inv_level);
	DDX_Control(pDX, INV_SIM_LENGTH, input_num_months);
	DDX_Control(pDX, INV_INPUT_DEMAND_SIZE, input_num_values_demand);
	DDX_Control(pDX, INV_NPUT_AVG_TIME_INTER, input_mean_interdemand);
	DDX_Control(pDX, INV_INPUT_SETUP_COST, input_setup_cost);	
	DDX_Control(pDX, INV_INPUT_INCRE_COST, input_incremental_cost);
	DDX_Control(pDX, INV_INPUT_HOLDING, input_holding_cost);
	DDX_Control(pDX, INV_INPUT_SHORTAGE, input_shortage_cost);
	DDX_Control(pDX, INV_INPUT_DELIVERY_START, input_minlag);
	DDX_Control(pDX, INV_INPUT_DELIVERY_END, input_maxlag);
	DDX_Control(pDX, IDC_EDIT16, input_num_policies);
	DDX_Control(pDX, INV_LIST_DIS_FUNC, distribution_demand_list);
	DDX_Control(pDX, INV_INPUT_INTERVAL, input_interval);
	DDX_Control(pDX, INV_INPUT_MAX_LEVEL, input_bigs);
	DDX_Control(pDX, INV_INPUT_MIN_LEVEL, input_smalls);
	OnBnClickedInitialBtn();
}


BEGIN_MESSAGE_MAP(InventoryDlg, CDialogEx)
	ON_BN_CLICKED(INV_POP_RESULT_LIST, &InventoryDlg::OnBnClickedPopResult)
	ON_BN_CLICKED(INV_RUN_BTN, &InventoryDlg::OnBnClickedRunBtn)
	ON_BN_CLICKED(INV_RESET_ZRNG, &InventoryDlg::OnBnClickedResetZrng)
	ON_BN_CLICKED(INV_DELETE_ALL_BTN, &InventoryDlg::OnBnClickedDeleteAllBtn)
	ON_BN_CLICKED(INV_DELETE_SINGLE_BTN, &InventoryDlg::OnBnClickedDeleteSingleBtn)
	ON_BN_CLICKED(INV_ADD_SEED_BTN, &InventoryDlg::OnBnClickedAddSeedBtn)
	ON_BN_CLICKED(INV_LOAD_DEFAULT_BTN, &InventoryDlg::OnBnClickedLoadDefaultBtn)
	ON_BN_CLICKED(INV_INITIAL_BTN, &InventoryDlg::OnBnClickedInitialBtn)
	ON_BN_CLICKED(INV_GEN_POLICY_BTN, &InventoryDlg::OnBnClickedGenPolicyBtn)
END_MESSAGE_MAP()

// InventoryDlg message handlers


void InventoryDlg::OnBnClickedPopResult()
{
	// TODO: Add your control notification handler code here
	InventoryResultListDlg newTag;
	newTag.DoModal();
}


void InventoryDlg::OnBnClickedRunBtn()
{
	// TODO: Add your control notification handler code here
	// clear all seeds
	std::fill(RandomGenerator::zrng.begin(), RandomGenerator::zrng.begin(), 0);
	// get all seed from randompool
	CString seedText;
	for(int i = 0; i < RandomSeedPool.GetCount(); i++)
	{
		RandomSeedPool.GetText(i, seedText);
		RandomGenerator::zrng.insert(RandomGenerator::zrng.begin(),0, _ttol(seedText));
	}

	RunButton.EnableWindow(0);
	{
		// collect the data from all input tags;

		CString initial_inv_level_cs;
		CString num_months_cs;
		CString num_policies_cs;
		CString num_values_demand_cs;
		CString mean_interdemand_cs;
		CString setup_cost_cs;
		CString incremental_cost_cs;
		CString holding_cost_cs;
		CString shortage_cost_cs;
		CString minlag_cs;
		CString maxlag_cs;

		input_initial_inv_level.GetWindowText(initial_inv_level_cs);
		input_num_months.GetWindowText(num_months_cs);
		input_num_values_demand.GetWindowText(num_values_demand_cs);
		input_mean_interdemand.GetWindowText(mean_interdemand_cs);
		input_setup_cost.GetWindowText(setup_cost_cs);
		input_incremental_cost.GetWindowText(incremental_cost_cs);
		input_holding_cost.GetWindowText(holding_cost_cs);
		input_shortage_cost.GetWindowText(shortage_cost_cs);
		input_minlag.GetWindowText(minlag_cs);
		input_maxlag.GetWindowText(maxlag_cs);
		input_num_policies.GetWindowText(num_policies_cs);


		this->data_handler.set_parameters(_ttoi(initial_inv_level_cs),_ttoi(num_months_cs),_ttoi(num_policies_cs),_ttoi(num_values_demand_cs),
			_ttof(mean_interdemand_cs),_ttof(setup_cost_cs),_ttof(incremental_cost_cs),_ttof(holding_cost_cs),
			_ttof(shortage_cost_cs),_ttof(minlag_cs),_ttof(maxlag_cs));

		int distribution_length = _ttoi(num_values_demand_cs);
		float distribution[BUFFER_SIZE];
		memset(distribution,1.0,BUFFER_SIZE);
		CString point_text;
		for(int i = 0; i < distribution_demand_list.GetCount(); i++)
		{
			distribution_demand_list.GetText(i, point_text);
			distribution[i] = _ttof(point_text);
		}

		this->data_handler.fill_prob_distri_function(distribution);

	}

	{
		// get the data from policy list
		std::vector<int> small_head_set;
		std::vector<int> big_head_set;
		for (Policy temp : this->current_policy_list){
			small_head_set.push_back(temp.small_head);
			big_head_set.push_back(temp.big_head);
		}
		this->data_handler.num_policies=current_policy_list.size();

		std::vector<PolicyRow> result = this->data_handler.loop(&small_head_set[0],&big_head_set[0]);
		InvReporter new_reporter;
		new_reporter.policy_rows = result;
	}


	RunButton.EnableWindow(1);

}


void InventoryDlg::OnBnClickedResetZrng()
{
	// TODO: Add your control notification handler code here
	OnBnClickedDeleteAllBtn();
	TCHAR buffer[BUFFER_SIZE];

	// TODO: Add your control notification handler code here
	for (auto seed : default_seeds){
		memset(buffer,0,BUFFER_SIZE);
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), seed);
		RandomSeedPool.AddString(buffer);
		RandomGenerator::zrng.push_back(seed);
	}
}


void InventoryDlg::OnBnClickedDeleteAllBtn()
{
	// TODO: Add your control notification handler code here
	RandomSeedPool.ResetContent();
	UpdateWindow();
}


void InventoryDlg::OnBnClickedDeleteSingleBtn()
{
	// TODO: Add your control notification handler code here
	CListBox * pList1 = (CListBox *)GetDlgItem(INV_LIST_SEED);

	int nSel = pList1->GetCurSel();
	if (nSel != LB_ERR)
	{
		CString ItemSelected; 
		pList1->GetText(nSel, ItemSelected);
		pList1->DeleteString(nSel);
	}
}


void InventoryDlg::OnBnClickedAddSeedBtn()
{
	// TODO: Add your control notification handler code here
	CString newSeed;
	input_seed.GetWindowText(newSeed);
	if (newSeed.IsEmpty()) AfxMessageBox(_T("Empty Seed"));
	else RandomSeedPool.AddString(newSeed);
}


void InventoryDlg::OnBnClickedLoadDefaultBtn()
{
	// TODO: Add your control notification handler code here
	OnBnClickedDeleteAllBtn();
	TCHAR buffer[BUFFER_SIZE];

	// TODO: Add your control notification handler code here
	for (auto seed : default_seeds){
		memset(buffer,0,BUFFER_SIZE);
		StringCchPrintf(buffer, sizeof(buffer)/sizeof(TCHAR), _T("%d"), seed);
		RandomSeedPool.AddString(buffer);
		RandomGenerator::zrng.push_back(seed);
	}
}


inline void addPolicy(int small_head,int big_head,int policy_count,CListCtrl* policy_list_controller,std::vector<Policy> *policies)
{
	CString smalls_cs;
	CString bigs_cs;

	smalls_cs.Format(_T("%d"),small_head);
	bigs_cs.Format(_T("%d"),big_head);
	CString policy_count_cs;
	policy_count_cs.Format(_T("%d"), policy_count);
	int index = policy_list_controller->InsertItem(policy_count,policy_count_cs);
	policy_list_controller->SetItemText(index,1,smalls_cs);
	policy_list_controller->SetItemText(index,2,bigs_cs);
	Policy policy;
	policy.small_head = small_head;
	policy.big_head = big_head;
	policies->push_back(policy);
}
void InventoryDlg::OnBnClickedInitialBtn()
{
	// TODO: Add your control notification handler code here
	OnBnClickedLoadDefaultBtn();
	input_rep.SetWindowText(_T("10"));
	input_initial_inv_level.SetWindowText(_T("60"));
	input_num_months.SetWindowText(_T("120"));
	input_num_values_demand.SetWindowText(_T("4"));
	input_mean_interdemand.SetWindowText(_T("0.1"));
	input_setup_cost.SetWindowText(_T("32.0"));
	input_incremental_cost.SetWindowTextW(_T("3.0"));
	input_holding_cost.SetWindowTextW(_T("1.0"));
	input_shortage_cost.SetWindowTextW(_T("5.0"));
	input_minlag.SetWindowTextW(_T("0.5"));
	input_maxlag.SetWindowTextW(_T("1.0"));
	{
		distribution_demand_list.ResetContent();
		distribution_demand_list.AddString(_T("0.167"));
		distribution_demand_list.AddString(_T("0.500"));
		distribution_demand_list.AddString(_T("0.833"));
		distribution_demand_list.AddString(_T("1.0"));
	}
	input_interval.SetWindowTextW(_T("20"));

	policy_count = 0;
	PolicyList.DeleteAllItems();
	UpdateWindow();
	CString policy_count_cs;
	{		
		// init the set
		input_smalls.SetWindowTextW(_T("20"));
		input_bigs.SetWindowTextW(_T("100"));
		policy_count=9;
		addPolicy(20,40,1,&PolicyList,&(this->current_policy_list));
		addPolicy(20,60,2,&PolicyList,&(this->current_policy_list));
		addPolicy(20,80,3,&PolicyList,&(this->current_policy_list));
		addPolicy(20,100,4,&PolicyList,&(this->current_policy_list));
		addPolicy(40,60,5,&PolicyList,&(this->current_policy_list));
		addPolicy(40,80,6,&PolicyList,&(this->current_policy_list));
		addPolicy(40,100,7,&PolicyList,&(this->current_policy_list));
		addPolicy(60,80,8,&PolicyList,&(this->current_policy_list));
		addPolicy(60,100,9,&PolicyList,&(this->current_policy_list));
	}
	policy_count_cs.Format(_T("%d"), policy_count);

	input_num_policies.SetWindowTextW(policy_count_cs);
	RunButton.EnableWindow(1);
}


void InventoryDlg::OnBnClickedGenPolicyBtn()
{
	CString smalls_cs;
	CString bigs_cs;
	input_smalls.GetWindowText(smalls_cs);
	input_bigs.GetWindowText(bigs_cs);

	if (smalls_cs.IsEmpty() || bigs_cs.IsEmpty()){
		AfxMessageBox(_T("Illegal Parameter, check the input"));
		return ;
	}

	int small_head = _ttoi(smalls_cs);
	int big_head = _ttoi(bigs_cs);

	policy_count++;
	addPolicy(small_head,big_head,policy_count,&PolicyList,&(this->current_policy_list));

	CString policy_count_cs;
	policy_count_cs.Format(_T("%d"), policy_count);
	input_num_policies.SetWindowTextW(policy_count_cs);
}
