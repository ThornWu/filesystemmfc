// MoveFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileSystem.h"
#include "MoveFileDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "file.h"
#include "manager.h"
#include <regex>


// MoveFileDlg �Ի���

IMPLEMENT_DYNAMIC(MoveFileDlg, CDialogEx)

MoveFileDlg::MoveFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MOVE, pParent)
{

}

MoveFileDlg::~MoveFileDlg()
{
}

void MoveFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILELIST, m_movelist);
	DDX_Control(pDX, IDC_EDIT2, m_movedst);
}


BEGIN_MESSAGE_MAP(MoveFileDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &MoveFileDlg::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_FILELIST, &MoveFileDlg::OnNMDblclkFilelist)
END_MESSAGE_MAP()


// MoveFileDlg ��Ϣ�������



BOOL MoveFileDlg::OnInitDialog()
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



	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	Init();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}
int MoveFileDlg::Init() {

	std::string pwd = "/";
	c_pwd = pwd.c_str();
	m_movelist.InsertColumn(0, _T("�ļ�������"), LVCFMT_LEFT, 200);
	m_movedst.SetWindowTextW(c_pwd);

	Refresh();
	m_movelist.SetExtendedStyle(m_movelist.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	return 0;
}

void MoveFileDlg::Refresh() {
	auto && manager = z::Manager::Instance();
	m_movelist.DeleteAllItems();
	int n = 0;


	CString file, size, user, type;
	std::string pwd = (CStringA)c_pwd;


	auto list = manager.ls(pwd);
	for (auto item : list) {		
		file = item.filename_.c_str();
		//�ļ���
		if (item.flag_ & 0x01) {
			m_movelist.InsertItem(n, file);
			n++;
		}
		else {
			continue;
		}	
	}
	
}

static auto && manager = z::Manager::Instance();
void MoveFileDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	

	std::string dst = (CStringA)c_pwd;
	std::string temp_path = (CStringA)sourcefilename;
	std::string path = temp_path;
	std::string dst_path = temp_path;
	std::string source = (CStringA)c_source;

	if (!z::Path::isFullPath(temp_path))
	{
		if (source == "/")
			path = source + temp_path;
		else
			path = source + "/" + temp_path;
	}

	//�ƶ��ļ����У�
	if (!iscopy) {
		if (!z::Path::isFullPath(dst)) dst = source + "/" + dst;

		manager.mv(path, dst);
	}
	else {
		if (!z::Path::isFullPath(temp_path)) 
		{
			if (dst == "/")
				dst_path = dst + temp_path;
			else
				dst_path = dst + "/" + temp_path;
		}
		
		if (path == dst_path) {
			dst_path = dst_path + "(1)";
		}

		if (!manager.cp(path, dst_path)) {
			MessageBox(_T("����ʧ��"));
		}
	}
	manager.destory();

	CDialogEx::OnOK();

}


void MoveFileDlg::OnNMDblclkFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	


	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	m_Row = pNMListView->iItem;//���ѡ�е���  

	CString dir = m_movelist.GetItemText(m_Row, 0);


	//˫���ǵ�Ԫ��Ŀ��ʱֱ�ӷ���
	if (m_Row == -1) {
		return;
	}

	auto && manager = z::Manager::Instance();
	std::string temp_path = (CStringA)dir;
	std::string path = temp_path;
	std::string pwd = (CStringA)c_pwd;

	//��Ŀ¼
	
		if (c_pwd == _T("/") && dir == ("..")) {

		}
		else {
			if (!z::Path::isFullPath(temp_path))
				path = pwd + "/" + temp_path;

			if (manager.exist(path))
				pwd = z::Path::parser(path);

			c_pwd = pwd.c_str();
			m_movedst.SetWindowTextW(c_pwd);

			Refresh();

		}


	*pResult = 0;
}
