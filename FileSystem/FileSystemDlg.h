
// FileSystemDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"



// CFileSystemDlg 对话框
class CFileSystemDlg : public CDialogEx
{
// 构造
public:
	CFileSystemDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILESYSTEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString ChildUser;
	CString ChildPass;
	CListCtrl m_list;
	int m_Row;//选中的列表行号
	int Init();
	afx_msg void OnDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult);
	CString c_pwd;
	CString c_in;
	CEdit m_edit;
	CEdit m_readtext;
	afx_msg void OnBnClickedAbout();
	CEdit m_filename;
	CButton m_closefile;
	afx_msg void OnBnClickedClosefile();
	afx_msg void OnBnClickedDelefile();
	void Refresh();
	afx_msg void OnBnClickedLogout();
	afx_msg void OnBnClickedSignup();
	afx_msg void OnBnClickedMovefile();
	afx_msg void OnBnClickedCopyfile();
	afx_msg void OnBnClickedNewfiles();
	afx_msg void OnBnClickedRenameafile();
};


