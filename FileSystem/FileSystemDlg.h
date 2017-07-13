
// FileSystemDlg.h : ͷ�ļ�
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"



// CFileSystemDlg �Ի���
class CFileSystemDlg : public CDialogEx
{
// ����
public:
	CFileSystemDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILESYSTEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString ChildUser;
	CString ChildPass;
	CListCtrl m_list;
	int m_Row;//ѡ�е��б��к�
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
	afx_msg void OnBnClickedNewfile();
	CEdit m_newfilename;
	bool isfile;
	int m_radio;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRenamefile();
	CEdit m_renamefile;
	afx_msg void OnBnClickedDelefile();
	void Refresh();
};

