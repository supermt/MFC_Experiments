// CoinFlipDlg.cpp : implementation file


#include "stdafx.h"
#include "MFC_Experiments.h"
#include "CoinFlipDlg.h"
#include "afxdialogex.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <numeric>


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
	DDX_Control(pDX, IDC_COST, input_cost_per_round);
	DDX_Control(pDX, IDC_REWARD, input_reward_per_game);
	DDX_Control(pDX, IDC_DIFF, input_condition);
	DDX_Control(pDX, IDC_WIN_ROUNDS, input_experiment_time);
	DDX_Control(pDX, IDC_EXPECTED_ROUNDS, output_expected_rounds);
	DDX_Control(pDX, IDC_EXPECTED_REWARD, output_expected_rewards);
	OnBnClickedInit();
}


BEGIN_MESSAGE_MAP(CoinFlipDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &CoinFlipDlg::OnBnClickedInit)
	ON_BN_CLICKED(IDC_BUTTON1, &CoinFlipDlg::OnBnClickedRun)
END_MESSAGE_MAP()


// CoinFlipDlg message handlers

inline bool toss(void) {
	int randomNumber;
	randomNumber = 1 + rand() % 2;

	return randomNumber == 1;
}


inline float avg(std::vector<float> *v)
{
	int n = v->size();	
	if (n==0){
		return 0;
	}

	return	accumulate(v->begin(), v->end(), 0.0) / n;

}
void CoinFlipDlg::OnBnClickedInit()
{
	// TODO: Add your control notification handler code here

	input_cost_per_round.SetWindowTextW(_T("1"));
	input_reward_per_game.SetWindowTextW(_T("3"));
	input_condition.SetWindowTextW(_T("3"));
	input_experiment_time.SetWindowTextW(_T("10"));
	output_expected_rewards.SetWindowTextW(_T("Wait for Calculation"));
	output_expected_rounds.SetWindowTextW(_T("Wait for Calculation"));
}


void CoinFlipDlg::OnBnClickedRun()
{
	// TODO: Add your control notification handler code here
	CString trigger_time_cs;
	CString cost_per_round_cs;
	CString reward_per_game_cs;
	CString diff_head_tail_cs;

	double throw_time = 0;


	input_cost_per_round.GetWindowTextW(cost_per_round_cs);
	input_reward_per_game.GetWindowTextW(reward_per_game_cs);
	input_condition.GetWindowTextW(diff_head_tail_cs);
	input_experiment_time.GetWindowTextW(trigger_time_cs);

	int trigger_time = _ttoi(trigger_time_cs);
	int diff_head_tail = _ttoi(diff_head_tail_cs);
	int reward_once = _ttoi(reward_per_game_cs);
	float reward_mean = 0;
	int cost_once = _ttoi(cost_per_round_cs);
	float cost_total = 0;

	std::vector<float> round_to_win_list;

	while(trigger_time>0){

		int head_count = 0;
		int tail_count = 0;
		int round_to_win = 0;
		while(1){
			throw_time+=1.0;
			round_to_win+=1.0;
			if (toss())head_count++;
			else tail_count++;
			if (abs(head_count-tail_count) >= diff_head_tail){ 
				round_to_win_list.push_back(round_to_win);
				break;
			}
		}
		trigger_time--;
	}

	double expected_round_to_win= avg(&round_to_win_list);

	double expected_reward =  (_ttoi(trigger_time_cs) * reward_once - throw_time * cost_once) / throw_time;

	CString expected_rounds_cs;
	expected_rounds_cs.Format(_T("%.2f"),expected_round_to_win);
	CString expected_rewards_cs;
	expected_rewards_cs.Format(_T("%f"),expected_reward);

	output_expected_rounds.SetWindowTextW(expected_rounds_cs);
	output_expected_rewards.SetWindowTextW(expected_rewards_cs);


}
