// SignUpDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileSystem.h"
#include "SignUpDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "file.h"
#include "manager.h"
#include <regex>


// SignUpDlg �Ի���

IMPLEMENT_DYNAMIC(SignUpDlg, CDialogEx)

SignUpDlg::SignUpDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SignUp, pParent)
{

}

SignUpDlg::~SignUpDlg()
{
}

void SignUpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, UserNameEdit, m_signname);
	DDX_Control(pDX, PasswordEdit, m_signpass);
	DDX_Control(pDX, PasswordEdit2, m_signrepeat);
}


BEGIN_MESSAGE_MAP(SignUpDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &SignUpDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &SignUpDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// SignUpDlg ��Ϣ�������

static auto && manager = z::Manager::Instance();

void SignUpDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString inputname, inputpass, inputrepeat;
	m_signname.GetWindowTextW(inputname);
	m_signpass.GetWindowTextW(inputpass);
	m_signrepeat.GetWindowTextW(inputrepeat);

	std::string s_inputname = (CStringA)inputname;
	std::string s_inputpass = (CStringA)inputpass;
	std::string s_current = (CStringA)this->currentuser;

	if (inputname == _T("")) {
		MessageBox(_T("�û�������Ϊ��"));
		return;
	}
	else if (inputname == _T("root")) {
		MessageBox(_T("�Ƿ����û���������������"));
		return;
	}
	else if (inputpass == _T("")) {
		MessageBox(_T("���벻��Ϊ��"));
		return;
	}
	else if (inputrepeat == _T("")) {
		MessageBox(_T("ȷ�����벻��Ϊ��"));
		return;
	}
	else if (inputpass != inputrepeat) {
		MessageBox(_T("�����������벻ͬ"));
		return;
	}
	else {
		if (s_current == "root") {
			if (!manager.signup(s_inputname, s_inputpass)) {
				MessageBox(_T("ע��ʧ��"));
			}
		}
		else {
			MessageBox(_T("Ȩ�޲���,ע��ʧ��"));
		}
	}

	CDialogEx::OnOK();
}


void SignUpDlg::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
