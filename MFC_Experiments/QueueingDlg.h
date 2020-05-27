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
};
