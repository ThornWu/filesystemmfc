#pragma once
#include "afxwin.h"


// RenameFileDlg 对话框

class RenameFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RenameFileDlg)

public:
	RenameFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RenameFileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_rename;
	CString c_pwd;
	CString oldname;
	afx_msg void OnBnClickedOk();
};
