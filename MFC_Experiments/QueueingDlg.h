#pragma once
#include "afxcmn.h"


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
};
