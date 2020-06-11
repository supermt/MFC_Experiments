#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "QueueingSolvers.h"

// QueueingDlg dialog

class QueueingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(QueueingDlg)

public:
	QueueingDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~QueueingDlg();

// Dialog Data
	enum { IDD = QueueingTag };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	std::vector<std::vector<float>> result_lines;
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	CListCtrl QueueingResultList;
	CButton fixed_customer_or_not;
	CButton fixed_length_or_not;
	afx_msg void OnBnClickedRun();
	afx_msg void OnBnClickedClearResult();
	afx_msg void OnBnClickedFixedCustomer();
	afx_msg void OnBnClickedFixedLength();
	int row_count;
	CButton RunButton;
	void add_result_line(ResultHandler *result,CListCtrl* list_controller,int row_count,int game_type);
	CListBox RandomSeedPool;
	afx_msg void OnBnClickedResetStream();
	afx_msg void OnBnClickedLoadDefault();
	void run_simulation_once(int seed,int game_type,float avg_arrive,float avg_service,float third_para);
	afx_msg void OnBnClickedClearSeedList();
	CEdit input_avg_interarrive;
	CEdit input_avg_service_time;
	CEdit input_customers;
	CEdit input_simulation_length;
	CEdit input_delay_limit;
	CEdit input_replication_times;
	afx_msg void OnBnClickedDeleteSelectedSeed();
	afx_msg void OnBnClickedAddSeed();
	CEdit input_seed;
	afx_msg void OnBnClickedLimiting();
	int Radio_origin;
	CEdit input_open_time;
	CEdit input_close_time;
	CEdit input_queue_capacity;
	afx_msg void OnBnClickedTimeRange();
	afx_msg void OnBnClickedRadioQueueLimited();
	void SetRepText();
	std::vector<int> seed_list;
};
