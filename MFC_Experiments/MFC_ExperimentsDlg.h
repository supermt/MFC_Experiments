
// MFC_ExperimentsDlg.h : header file
//

#pragma once


// CMFC_ExperimentsDlg dialog
class CMFC_ExperimentsDlg : public CDHtmlDialog
{
// Construction
public:
	CMFC_ExperimentsDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFC_EXPERIMENTS_DIALOG, IDH = IDR_HTML_MFC_EXPERIMENTS_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	afx_msg void OnBnClickedEnterCoin();
	afx_msg void OnBnClickedEnterQueue();
	afx_msg void OnBnClickedEnterInvenory();
};
