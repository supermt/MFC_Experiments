#pragma once


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
};
