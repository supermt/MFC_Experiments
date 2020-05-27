#pragma once
#include "afxwin.h"


// CoinFlipDlg dialog

class CoinFlipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CoinFlipDlg)

public:
	CoinFlipDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CoinFlipDlg();

// Dialog Data
	enum { IDD = CoinFlipTag };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit input_cost_per_round;
	CEdit input_reward_per_game;
	CEdit input_condition;
	CEdit input_experiment_time;
	CEdit output_expected_rounds;
	CEdit output_expected_rewards;
	afx_msg void OnBnClickedInit();
	afx_msg void OnBnClickedRun();
};
