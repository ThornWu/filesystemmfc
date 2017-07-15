#pragma once
#include "afxwin.h"


// NewFileDlg 对话框

class NewFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(NewFileDlg)

public:
	NewFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~NewFileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWFILE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL NewFileDlg::OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CEdit m_newfile;
	CString c_pwd;
	afx_msg void OnBnClickedOk();
	bool isfile;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	int m_newtype;
};
