#pragma once


// SignUpDlg �Ի���

class SignUpDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SignUpDlg)

public:
	SignUpDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~SignUpDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SignUp };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
