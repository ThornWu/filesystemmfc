
// FileSystemDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileSystem.h"
#include "FileSystemDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "file.h"
#include "manager.h"
#include <regex>
#include "LoginDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileSystemDlg �Ի���



CFileSystemDlg::CFileSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_FILESYSTEM_DIALOG, pParent)
	, m_radio(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_FILELIST, m_list);
	DDX_Control(pDX, IDC_EDIT2, m_edit);
	DDX_Control(pDX, IDC_EDIT1, m_readtext);
	DDX_Control(pDX, IDC_EDIT3, m_filename);
	DDX_Control(pDX, IDC_BUTTON2, m_closefile);
	DDX_Control(pDX, IDC_EDIT4, m_newfilename);
	DDX_Radio(pDX, IDC_RADIO1, m_radio);
	DDX_Control(pDX, IDC_EDIT5, m_renamefile);
}

BEGIN_MESSAGE_MAP(CFileSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_NOTIFY(NM_DBLCLK, IDC_FILELIST, &CFileSystemDlg::OnDblclkFilelist)
	ON_BN_CLICKED(About, &CFileSystemDlg::OnBnClickedAbout)
	ON_BN_CLICKED(CloseFile, &CFileSystemDlg::OnBnClickedClosefile)
	ON_BN_CLICKED(NewFile, &CFileSystemDlg::OnBnClickedNewfile)
	ON_BN_CLICKED(IDC_RADIO1, &CFileSystemDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CFileSystemDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(RenameFile, &CFileSystemDlg::OnBnClickedRenamefile)
	ON_BN_CLICKED(DeleFile, &CFileSystemDlg::OnBnClickedDelefile)
	ON_BN_CLICKED(LOGOUT, &CFileSystemDlg::OnBnClickedLogout)
END_MESSAGE_MAP()


// CFileSystemDlg ��Ϣ�������

BOOL CFileSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	GetDlgItem(UserStatic)->SetWindowTextW(ChildUser);
	Init();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CFileSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CFileSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CFileSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





static auto && manager = z::Manager::Instance();

int CFileSystemDlg::Init()
{



	//������ʶ��
	USES_CONVERSION;

	std::string s_inputname = (CStringA)this->ChildUser;
	std::string s_inputpass = (CStringA)this->ChildPass;





	manager.load_users();
	if (!manager.login(s_inputname, s_inputpass))
		return -1;
	//	do {
	//		std::cout << "login$ username:"; std::cin >> username;
	//		std::cout << "login$ password:"; std::cin >> password;
	//	} while (!manager.login(username, password));

	




	//std::string username, password;
	//	do {
	//		std::cout << "login$ username:"; std::cin >> username;
	//		std::cout << "login$ password:"; std::cin >> password;
	//	} while (!fs->login(username, password));








	std::string pwd = "/";
	std::string in = "";



	m_list.InsertColumn(0, _T("�ļ���"), LVCFMT_LEFT, 110);
	m_list.InsertColumn(1, _T("����ʱ��"), LVCFMT_LEFT, 180);
	m_list.InsertColumn(2, _T("�ļ�����"), LVCFMT_LEFT,90);
	m_list.InsertColumn(3, _T("�ļ���С"), LVCFMT_LEFT,90);
	m_list.InsertColumn(4, _T("������"), LVCFMT_LEFT, 80);


	c_pwd = pwd.c_str();
	c_in = in.c_str();

	m_edit.SetWindowTextW(c_pwd);
	isfile = true;

	Refresh();

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	

	/**
	CString strData = TEXT("hello ����");
	std::string strTempData = (CStringA)strData;

	std::string strData1 = "hello ����";
	CString strTempData1(strData1.c_str());
	**/



	/**
	
	else if (cmd[0] == "rename") {
	if (cmd.size() < 3) continue;

	auto temp_path = cmd[1];
	if (!z::Path::isFullPath(cmd[1])) temp_path = pwd + "/" + cmd[1];

	if (!manager.rename(temp_path, cmd[2]))
	std::cout << "������ʧ��." << std::endl;
	}

	//ɾ��
	else if (cmd[0] == "rm") {
	if (cmd.size() < 2) continue;

	auto temp_path = cmd[1];
	if (!z::Path::isFullPath(cmd[1])) temp_path = pwd + "/" + cmd[1];

	if (!manager.rm(temp_path))
	std::cout << "ɾ��ʧ��." << std::endl;
	}
	for (std::string::size_type i = 1; i < cmd.size(); ++i) {

	auto temp_path = cmd[1];
	if (!z::Path::isFullPath(cmd[1])) temp_path = pwd + "/" + cmd[1];
	manager.mkdirs(temp_path);
	}
	}
	else if (cmd[0] == "help") {
		std::cout << "\tcp <path> <path> : ����" << std::endl;
		std::cout << "\tmv <path> <path> : �ƶ�" << std::endl;
		std::cout << "\trm <path> : ɾ��" << std::endl;
		std::cout << "\trename <path> <path> : ������" << std::endl;
	}
	**/
	return 0;
}



void CFileSystemDlg::OnDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_Row = pNMListView->iItem;//���ѡ�е���  
	
	CString dir=m_list.GetItemText(m_Row, 0);
	CString size = m_list.GetItemText(m_Row,3);

	auto && manager = z::Manager::Instance();
	std::string temp_path = (CStringA)dir;
	std::string path=temp_path;
	std::string pwd = (CStringA)c_pwd;

	//��Ŀ¼
	if (size == _T(" ")) {
		if (c_pwd == _T("/") && dir == ("..")) {

		}
		else {
			if (!z::Path::isFullPath(temp_path)) 
				path = pwd + "/" + temp_path;

			if (manager.exist(path))
				pwd = z::Path::parser(path);

			c_pwd = pwd.c_str();
			m_edit.SetWindowTextW(c_pwd);

			Refresh();
		
		}
	}
	
	
	// �鿴�ļ�
	else if (size != _T(" ")) {
		z::File file(pwd + "/" + temp_path, z::File::in);
		if (!file.is_open()) {
			MessageBox(_T("�ļ���ʧ��"));
			return;
		}
		

		auto buf_size = file.size() + 1;
		auto buf = new char[buf_size];
		memset(buf, 0, buf_size);
		file.read(buf, file.size());
		std::string text = buf;


		CString c_text(text.c_str());
		m_filename.SetWindowTextW(_T("�ļ�����")+dir);
		m_readtext.SetWindowTextW(c_text);
		m_readtext.ShowWindow(SW_SHOW);
		m_filename.ShowWindow(SW_SHOW);
		m_closefile.ShowWindow(SW_SHOW);
		
	}

	*pResult = 0;
}


void CFileSystemDlg::OnBnClickedAbout()
{
	CAboutDlg dlg;

	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}
}


void CFileSystemDlg::OnBnClickedClosefile()
{
	m_readtext.ShowWindow(SW_HIDE);
	m_filename.ShowWindow(SW_HIDE);
	m_closefile.ShowWindow(SW_HIDE);
}



void CFileSystemDlg::OnBnClickedNewfile()
{
	// TODO: �ڴ���ӿؼ�֪ͨ���������롣

	auto && manager = z::Manager::Instance();

	bool a = isfile;
	CString filename;
	m_newfilename.GetWindowTextW(filename);
	std::string temp_path = (CStringA)filename;
	std::string path = temp_path;
	std::string pwd = (CStringA)c_pwd;

	if (filename.IsEmpty()){
		if (isfile)
			MessageBox(_T("�ļ������Ʋ���Ϊ��"));
		else
			MessageBox(_T("�ļ������Ʋ���Ϊ��"));
	}
	else {
		if (isfile) {
				if (!z::Path::isFullPath(temp_path)) path = pwd + "/" + temp_path;
				manager.touch(path);
		}
		else {
			if (!z::Path::isFullPath(temp_path)) path = pwd + "/" + temp_path;
			manager.mkdirs(path);
		}

		c_pwd = pwd.c_str();
		m_edit.SetWindowTextW(c_pwd);
		Refresh();
		

	}
	m_newfilename.SetWindowTextW(_T(""));

}

void CFileSystemDlg::OnBnClickedRadio1()
{
	isfile = true;
}


void CFileSystemDlg::OnBnClickedRadio2()
{
	isfile = false;
}


void CFileSystemDlg::OnBnClickedRenamefile()
{
	auto && manager = z::Manager::Instance();
	CString filename;
	m_renamefile.GetWindowTextW(filename);
	if (filename.IsEmpty()) {
		MessageBox(_T("�ļ����Ʋ���Ϊ��"));
		return;
	}
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(_T("������ѡ��һ��"));
		return;
	}
	//�õ��кţ�ͨ��POSITIONת��
	int nId = (int)m_list.GetNextSelectedItem(pos);
	if (nId < 2) {
		MessageBox(_T("����ܱ��޸ģ�������ѡ��"));
		return;
	}
	else {
		CString dir = m_list.GetItemText(nId, 0);
		std::string pwd = (CStringA)c_pwd;
		std::string oldfile = (CStringA)dir;
		std::string newfile = (CStringA)filename;
		std::string path = (CStringA)filename;

		if (!z::Path::isFullPath(oldfile)) path = pwd + "/" + oldfile;

		if (!manager.rename(path, newfile)) {
			MessageBox(_T("������ʧ��"));
		}
		else {
			MessageBox(_T("�޸ĳɹ�"));
			Refresh();
			m_renamefile.SetWindowTextW(_T(""));
		}
	}

}


void CFileSystemDlg::OnBnClickedDelefile()
{
	auto && manager = z::Manager::Instance();
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(_T("������ѡ��һ��"));
		return;
	}
	//�õ��кţ�ͨ��POSITIONת��
	int nId = (int)m_list.GetNextSelectedItem(pos);
	if (nId < 2) {
		MessageBox(_T("����ܱ��޸ģ�������ѡ��"));
		return;
	}
	else {
		CString dir = m_list.GetItemText(nId, 0);
		std::string pwd = (CStringA)c_pwd;
		std::string file = (CStringA)dir;
		std::string path = file;

		if (!z::Path::isFullPath(file))
			path = pwd + "/" + file;
		

		if (!manager.rm(path)) {
			MessageBox(_T("ɾ��ʧ��"));
		}
		else {
			MessageBox(_T("ɾ���ɹ�"));
			Refresh();
		}
	
	}
}


void CFileSystemDlg::Refresh() {
	auto && manager = z::Manager::Instance();
	m_list.DeleteAllItems();
	int n = 0;
	char buf[20];

	CString file, size, user, type;
	std::string pwd = (CStringA)c_pwd;
	auto list = manager.ls(pwd);
	for (auto item : list) {

		strftime(buf, 20, "%Y-%m-%d %H:%M:%S", localtime(reinterpret_cast<time_t const *>(&item.mtime_)));
		CString time(buf);
		file = item.filename_.c_str();
		user = item.username_.c_str();
		if (item.flag_ & 0x01) {
			size = _T(" ");
			type = "�ļ���";
		}
		else {
			size.Format(_T("%d"), item.size_);
			type = "�ļ�";
		}
		m_list.InsertItem(n, file);
		m_list.SetItemText(n, 1, time);
		m_list.SetItemText(n, 2, type);
		m_list.SetItemText(n, 3, size);
		m_list.SetItemText(n, 4, user);
		n++;
	}

}

void CFileSystemDlg::OnBnClickedLogout()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	manager.destory();
	CDialog::OnOK();

	LoginDlg dlg;
	


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
