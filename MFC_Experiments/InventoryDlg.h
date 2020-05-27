#pragma once
#include "afxcmn.h"

#include "InventoryResultListDlg.h"
#include "Inv_handler.h"
#include "RandomStream.h"
#include "afxwin.h"

// InventoryDlg dialog

struct Policy
{
	int small_head;
	int big_head;
};

class InventoryDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InventoryDlg)

public:
	InventoryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~InventoryDlg();

	// Dialog Data
	enum { IDD = InventoryTag };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl PolicyList;
	afx_msg void OnBnClickedPopResult();
private:
	Inv_handler handler;
public:
	afx_msg void OnBnClickedRunBtn();
	CListBox RandomSeedPool;
	CButton RunButton;
	afx_msg void OnBnClickedResetZrng();
	afx_msg void OnBnClickedDeleteAllBtn();
	afx_msg void OnBnClickedDeleteSingleBtn();
	afx_msg void OnBnClickedAddSeedBtn();
	CEdit input_seed;
	afx_msg void OnBnClickedLoadDefaultBtn();
	Inv_handler data_handler;
	afx_msg void OnBnClickedInitialBtn();
	CEdit input_rep;
	CEdit initial_inv_level;
	CEdit input_num_months;
	CEdit input_num_values_demand;
	CEdit input_mean_interdemand;
	CEdit input_setup_cost;
	CEdit input_incremental_cost;
	CEdit input_holding_cost;
	CEdit input_shortage_cost;
	CEdit input_minlag;
	CEdit input_maxlag;
	CEdit input_num_policies;
	CListBox distribution_demand_list;
	CEdit input_interval;
	afx_msg void OnBnClickedGenPolicyBtn();

	int policy_count;
	CEdit input_bigs;
	CEdit input_smalls;
	std::vector<Policy> current_policy_list;
};
