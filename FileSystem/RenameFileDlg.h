#pragma once
#include "afxwin.h"


// RenameFileDlg �Ի���

class RenameFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RenameFileDlg)

public:
	RenameFileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~RenameFileDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_rename;
	CString c_pwd;
	CString oldname;
	afx_msg void OnBnClickedOk();
};
