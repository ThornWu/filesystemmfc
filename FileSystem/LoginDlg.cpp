// LoginDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileSystem.h"
#include "LoginDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "file.h"
#include "manager.h"
#include <regex>
#include "FileSystemDlg.h"


// LoginDlg 对话框

IMPLEMENT_DYNAMIC(LoginDlg, CDialogEx)

LoginDlg::LoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_LOGIN, pParent)
{

}

LoginDlg::~LoginDlg()
{
}

void LoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, UserNameEdit, m_user);
	DDX_Control(pDX, PasswordEdit, m_pass);
}


BEGIN_MESSAGE_MAP(LoginDlg, CDialogEx)
	
	ON_BN_CLICKED(IDLOGIN, &LoginDlg::OnBnClickedLogin)
END_MESSAGE_MAP()


// LoginDlg 消息处理程序




static auto && manager = z::Manager::Instance();
void LoginDlg::OnBnClickedLogin()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString c_inputname, c_inputpass;
	m_user.GetWindowTextW(c_inputname);
	m_pass.GetWindowTextW(c_inputpass);
	std::string s_inputname = (CStringA)c_inputname;
	std::string s_inputpass = (CStringA)c_inputpass;

	if (c_inputname == _T("")) {
		MessageBox(_T("用户名不能为空"));
		m_user.GetFocus();
		return;
	}
	else if(c_inputpass==_T("")){
		MessageBox(_T("密码不能为空"));
		m_pass.GetFocus();
		return;
	}

	
	manager.load_users();



	if (!manager.login(s_inputname, s_inputpass)) {
		MessageBox(TEXT("用户名或密码错误，请重新输入！"));
		m_user.SetWindowTextW(_T(""));
		m_pass.SetWindowTextW(_T(""));
		m_user.SetFocus();
		return;
	}
	else {
		manager.destory();
		CDialog::OnOK();

		CFileSystemDlg dlg;
		m_user.GetWindowTextW(dlg.ChildUser);
		m_pass.GetWindowTextW(dlg.ChildPass);


		INT_PTR nResponse = dlg.DoModal();
		if (nResponse == IDOK)
		{
			// TODO: 在此放置处理何时用
			//  “确定”来关闭对话框的代码
		}
		else if (nResponse == IDCANCEL)
		{

		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
			TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
		}
	}


	/**
		
//	do {
//		std::cout << "login$ username:"; std::cin >> username;
//		std::cout << "login$ password:"; std::cin >> password;
//	} while (!manager.login(username, password));

	z::File test("/home/test/ttt.txt");
	test.write("Hello", 5);
	test.close();


	auto rv = exe_cmd();

	
	
	**/


	/**
	this->UpdateData(true);
	CWnd *pWnd = NULL;
	bool isfind = false;

	char charInFile, charReturn;
	errno_t err;
	user User;
	err = fopen_s(&fp, "2.c", "r");
	if (err != 0) {
		MessageBox(TEXT("文件打开失败"));
	}
	else {
		while ((charInFile = getc(fp)) != EOF) {
			charReturn = sortChar(charInFile);
			while (charReturn != 0) {
				charReturn = sortChar(charReturn);
			}
		}
		fclose(fp);

		vector<user>::iterator ut = usr.begin();
		for (unsigned int count = 0; count < usr.size(); count++) {
			if (this->m_user == ut[count].name&&
				this->m_password == ut[count].pass) {
				isfind = true;
				break;

			}
		}
		if (isfind) {
			
			
		}
		else {

		}
	}
	**/
}
