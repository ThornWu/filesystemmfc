#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// MoveFileDlg �Ի���

class MoveFileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MoveFileDlg)

public:
	MoveFileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MoveFileDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MOVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
	int m_Row;//ѡ�е��б��к�
	bool iscopy;
	afx_msg void OnNMDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult);
};
