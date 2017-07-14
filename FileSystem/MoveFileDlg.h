#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// MoveFileDlg 对话框

class MoveFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MoveFileDlg)

public:
	MoveFileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MoveFileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL MoveFileDlg::OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_movelist;
	afx_msg void OnBnClickedOk();
	int Init();
	void Refresh();
	CString c_source;
	CString c_pwd;
	CString sourcefilename;
	CEdit m_movedst;
	int m_Row;//选中的列表行号
	bool iscopy;
	afx_msg void OnNMDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult);
};
