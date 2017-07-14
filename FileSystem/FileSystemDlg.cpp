
// FileSystemDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CFileSystemDlg 对话框



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


// CFileSystemDlg 消息处理程序

BOOL CFileSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GetDlgItem(UserStatic)->SetWindowTextW(ChildUser);
	Init();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}





static auto && manager = z::Manager::Instance();

int CFileSystemDlg::Init()
{



	//声明标识符
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



	m_list.InsertColumn(0, _T("文件名"), LVCFMT_LEFT, 110);
	m_list.InsertColumn(1, _T("创建时间"), LVCFMT_LEFT, 180);
	m_list.InsertColumn(2, _T("文件类型"), LVCFMT_LEFT,90);
	m_list.InsertColumn(3, _T("文件大小"), LVCFMT_LEFT,90);
	m_list.InsertColumn(4, _T("创建者"), LVCFMT_LEFT, 80);


	c_pwd = pwd.c_str();
	c_in = in.c_str();

	m_edit.SetWindowTextW(c_pwd);
	isfile = true;

	Refresh();

	m_list.SetExtendedStyle(m_list.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	

	/**
	CString strData = TEXT("hello 世界");
	std::string strTempData = (CStringA)strData;

	std::string strData1 = "hello 世界";
	CString strTempData1(strData1.c_str());
	**/



	/**
	
	else if (cmd[0] == "rename") {
	if (cmd.size() < 3) continue;

	auto temp_path = cmd[1];
	if (!z::Path::isFullPath(cmd[1])) temp_path = pwd + "/" + cmd[1];

	if (!manager.rename(temp_path, cmd[2]))
	std::cout << "重命名失败." << std::endl;
	}

	//删除
	else if (cmd[0] == "rm") {
	if (cmd.size() < 2) continue;

	auto temp_path = cmd[1];
	if (!z::Path::isFullPath(cmd[1])) temp_path = pwd + "/" + cmd[1];

	if (!manager.rm(temp_path))
	std::cout << "删除失败." << std::endl;
	}
	for (std::string::size_type i = 1; i < cmd.size(); ++i) {

	auto temp_path = cmd[1];
	if (!z::Path::isFullPath(cmd[1])) temp_path = pwd + "/" + cmd[1];
	manager.mkdirs(temp_path);
	}
	}
	else if (cmd[0] == "help") {
		std::cout << "\tcp <path> <path> : 复制" << std::endl;
		std::cout << "\tmv <path> <path> : 移动" << std::endl;
		std::cout << "\trm <path> : 删除" << std::endl;
		std::cout << "\trename <path> <path> : 重命名" << std::endl;
	}
	**/
	return 0;
}



void CFileSystemDlg::OnDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_Row = pNMListView->iItem;//获得选中的行  
	
	CString dir=m_list.GetItemText(m_Row, 0);
	CString size = m_list.GetItemText(m_Row,3);

	auto && manager = z::Manager::Instance();
	std::string temp_path = (CStringA)dir;
	std::string path=temp_path;
	std::string pwd = (CStringA)c_pwd;

	//打开目录
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
	
	
	// 查看文件
	else if (size != _T(" ")) {
		z::File file(pwd + "/" + temp_path, z::File::in);
		if (!file.is_open()) {
			MessageBox(_T("文件打开失败"));
			return;
		}
		

		auto buf_size = file.size() + 1;
		auto buf = new char[buf_size];
		memset(buf, 0, buf_size);
		file.read(buf, file.size());
		std::string text = buf;


		CString c_text(text.c_str());
		m_filename.SetWindowTextW(_T("文件名：")+dir);
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
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
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
	// TODO: 在此添加控件通知处理程序代码。

	auto && manager = z::Manager::Instance();

	bool a = isfile;
	CString filename;
	m_newfilename.GetWindowTextW(filename);
	std::string temp_path = (CStringA)filename;
	std::string path = temp_path;
	std::string pwd = (CStringA)c_pwd;

	if (filename.IsEmpty()){
		if (isfile)
			MessageBox(_T("文件夹名称不能为空"));
		else
			MessageBox(_T("文件夹名称不能为空"));
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
		MessageBox(_T("文件名称不能为空"));
		return;
	}
	POSITION pos = m_list.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		MessageBox(_T("请至少选择一项"));
		return;
	}
	//得到行号，通过POSITION转化
	int nId = (int)m_list.GetNextSelectedItem(pos);
	if (nId < 2) {
		MessageBox(_T("该项不能被修改，请重新选择"));
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
			MessageBox(_T("重命名失败"));
		}
		else {
			MessageBox(_T("修改成功"));
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
		MessageBox(_T("请至少选择一项"));
		return;
	}
	//得到行号，通过POSITION转化
	int nId = (int)m_list.GetNextSelectedItem(pos);
	if (nId < 2) {
		MessageBox(_T("该项不能被修改，请重新选择"));
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
			MessageBox(_T("删除失败"));
		}
		else {
			MessageBox(_T("删除成功"));
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
			type = "文件夹";
		}
		else {
			size.Format(_T("%d"), item.size_);
			type = "文件";
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
	// TODO: 在此添加控件通知处理程序代码
	manager.destory();
	CDialog::OnOK();

	LoginDlg dlg;
	


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
