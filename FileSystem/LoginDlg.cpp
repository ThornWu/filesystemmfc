// LoginDlg.cpp : ʵ���ļ�
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


// LoginDlg �Ի���

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


// LoginDlg ��Ϣ�������




static auto && manager = z::Manager::Instance();
void LoginDlg::OnBnClickedLogin()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	CString c_inputname, c_inputpass;
	m_user.GetWindowTextW(c_inputname);
	m_pass.GetWindowTextW(c_inputpass);
	std::string s_inputname = (CStringA)c_inputname;
	std::string s_inputpass = (CStringA)c_inputpass;

	if (c_inputname == _T("")) {
		MessageBox(_T("�û�������Ϊ��"));
		m_user.GetFocus();
		return;
	}
	else if(c_inputpass==_T("")){
		MessageBox(_T("���벻��Ϊ��"));
		m_pass.GetFocus();
		return;
	}

	
	manager.load_users();



	if (!manager.login(s_inputname, s_inputpass)) {
		MessageBox(TEXT("�û���������������������룡"));
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
			// TODO: �ڴ˷��ô����ʱ��
			//  ��ȷ�������رնԻ���Ĵ���
		}
		else if (nResponse == IDCANCEL)
		{

		}
		else if (nResponse == -1)
		{
			TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
			TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
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
		MessageBox(TEXT("�ļ���ʧ��"));
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
