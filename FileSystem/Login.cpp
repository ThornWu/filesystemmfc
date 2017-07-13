// Login.cpp : ʵ���ļ�
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
//state==0 ��ͨ״̬ state==1 �ַ���״̬
static int state;
//stringstate==0 �û��� stringstate==1 ����
static int stringstate;
// CLogin �Ի���

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


// CLogin ��Ϣ�������


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
			CDialog::OnOK();

			CFileSystemDlg dlg;
			dlg.ChildUser = m_user;
			dlg.ChildPass = m_password;

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
		else {
			MessageBox(TEXT("�û���������������������룡"));
			m_user = "";
			m_password = "";
			pWnd = GetDlgItem(UserNameEdit); // ��ȡIDC_USER�Ŀؼ�ָ��
			pWnd->SetFocus(); // ���ý���
			UpdateData(false);
		}
	}
}



	


void CLogin::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	AfxGetMainWnd()->PostMessage(WM_QUIT); // ����WM_QUIT��Ϣ�˳�����
	CDialogEx::OnCancel();
}




char CLogin::sortChar(char ch)
{
	char nextChar;
	int countChar = 0;//�ַ�������
	int countNum = 0;//���ּ���

	if (ch == ' ' || ch == '\'' || ch == ',')
		return 0;//���ַ����ڵĿո�ֱ���˳��ӳ���
	else if (ch == '\n') {
		state = 0; //state==0 ��ͨ״̬
	}
	else if (ch == '\"') {
		if (state == 0)
			state = 1;//state==1 �ַ���״̬
	}
	else if (ch == ';') {
		usr.push_back(User);
	}
	//������ĸʱ�Ĵ���(������/�ؼ���)
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

	//���������ַ�
	else {

	}
	return 0;
}

void CLogin::stringType()
{
	//�ж��Ƿ�Ϊ�ؼ��֣����򷵻�0�����򷵻�1
	if (strcmp(temp, "username") == 0) {
		stringstate = 0;
	}
	else if (strcmp(temp, "password") == 0) {
		stringstate = 1;
	}
}

void CLogin::resetWord()
{
	//�������
	for (int i = 0; i < MAX_CHAR_SIZE; i++)
		temp[i] = '\0';
	state = 0;
}

