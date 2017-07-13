// Login.cpp : 实现文件
//

#include "stdafx.h"
#include "FileSystem.h"
#include "Login.h"
#include "afxdialogex.h"
#include <vector>
#include "FileSystemDlg.h"

using namespace std;
vector<user> usr;
user User;
//state==0 普通状态 state==1 字符串状态
static int state;
//stringstate==0 用户名 stringstate==1 密码
static int stringstate;
// CLogin 对话框

IMPLEMENT_DYNAMIC(CLogin, CDialogEx)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialogEx(Login, pParent)
	, m_user(_T(""))
	, m_password(_T(""))
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, UserNameEdit, m_user);
	DDX_Text(pDX, PasswordEdit, m_password);
}


BEGIN_MESSAGE_MAP(CLogin, CDialogEx)
	ON_BN_CLICKED(IDYES, &CLogin::OnBnClickedYes)
	ON_BN_CLICKED(IDCANCEL, &CLogin::OnBnClickedCancel)
END_MESSAGE_MAP()


// CLogin 消息处理程序


void CLogin::OnBnClickedYes()
{

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
			CDialog::OnOK();

			CFileSystemDlg dlg;
			dlg.ChildUser = m_user;
			dlg.ChildPass = m_password;

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
		else {
			MessageBox(TEXT("用户名或密码错误，请重新输入！"));
			m_user = "";
			m_password = "";
			pWnd = GetDlgItem(UserNameEdit); // 获取IDC_USER的控件指针
			pWnd->SetFocus(); // 设置焦点
			UpdateData(false);
		}
	}
}



	


void CLogin::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxGetMainWnd()->PostMessage(WM_QUIT); // 发送WM_QUIT消息退出程序
	CDialogEx::OnCancel();
}




char CLogin::sortChar(char ch)
{
	char nextChar;
	int countChar = 0;//字符串计数
	int countNum = 0;//数字计数

	if (ch == ' ' || ch == '\'' || ch == ',')
		return 0;//非字符串内的空格直接退出子程序
	else if (ch == '\n') {
		state = 0; //state==0 普通状态
	}
	else if (ch == '\"') {
		if (state == 0)
			state = 1;//state==1 字符串状态
	}
	else if (ch == ';') {
		usr.push_back(User);
	}
	//读入字母时的处理(变量名/关键字)
	else if ((ch >= 'A'&&ch <= 'Z') || (ch >= 'a'&&ch <= 'z')) {
		temp[countChar] = ch;
		countChar++;
		while ((nextChar = getc(fp)) != '#') {
			if ((nextChar != '\"') && (nextChar != ':')) {
				temp[countChar] = nextChar;
				countChar++;
			}
			else {
				stringType();
				if (stringstate == 0 && state == 1) {
					User.name = temp;
				}
				else if (stringstate == 1 && state == 1) {
					User.pass = temp;
				}
				resetWord();
				return nextChar;
			}
		}
		return 0;
	}

	//读入其它字符
	else {

	}
	return 0;
}

void CLogin::stringType()
{
	//判断是否为关键字，是则返回0，否则返回1
	if (strcmp(temp, "username") == 0) {
		stringstate = 0;
	}
	else if (strcmp(temp, "password") == 0) {
		stringstate = 1;
	}
}

void CLogin::resetWord()
{
	//清空数组
	for (int i = 0; i < MAX_CHAR_SIZE; i++)
		temp[i] = '\0';
	state = 0;
}

