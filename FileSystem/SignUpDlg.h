#pragma once
#include "afxwin.h"


// SignUpDlg 对话框

class SignUpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SignUpDlg)

public:
	SignUpDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~SignUpDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SignUp };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_signname;
	CEdit m_signpass;
	CEdit m_signrepeat;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString currentuser;
};
