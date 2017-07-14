// SignUpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileSystem.h"
#include "SignUpDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "file.h"
#include "manager.h"
#include <regex>


// SignUpDlg 对话框

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


// SignUpDlg 消息处理程序

static auto && manager = z::Manager::Instance();

void SignUpDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CString inputname, inputpass, inputrepeat;
	m_signname.GetWindowTextW(inputname);
	m_signpass.GetWindowTextW(inputpass);
	m_signrepeat.GetWindowTextW(inputrepeat);

	std::string s_inputname = (CStringA)inputname;
	std::string s_inputpass = (CStringA)inputpass;
	std::string s_current = (CStringA)this->currentuser;

	if (inputname == _T("")) {
		MessageBox(_T("用户名不能为空"));
		return;
	}
	else if (inputname == _T("root")) {
		MessageBox(_T("非法的用户名，请重新输入"));
		return;
	}
	else if (inputpass == _T("")) {
		MessageBox(_T("密码不能为空"));
		return;
	}
	else if (inputrepeat == _T("")) {
		MessageBox(_T("确认密码不能为空"));
		return;
	}
	else if (inputpass != inputrepeat) {
		MessageBox(_T("两次密码输入不同"));
		return;
	}
	else {
		if (s_current == "root") {
			if (!manager.signup(s_inputname, s_inputpass)) {
				MessageBox(_T("注册失败"));
			}
		}
		else {
			MessageBox(_T("权限不足,注册失败"));
		}
	}

	CDialogEx::OnOK();
}


void SignUpDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
