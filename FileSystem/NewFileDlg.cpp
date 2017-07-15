// NewFileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FileSystem.h"
#include "NewFileDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "file.h"
#include "manager.h"
#include <regex>


// NewFileDlg �Ի���

IMPLEMENT_DYNAMIC(NewFileDlg, CDialogEx)

NewFileDlg::NewFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEWFILE, pParent)
	, m_newtype(0)
{

}

NewFileDlg::~NewFileDlg()
{
}

void NewFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT4, m_newfile);
	DDX_Radio(pDX, IDC_RADIO1, m_newtype);
}


BEGIN_MESSAGE_MAP(NewFileDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &NewFileDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_RADIO1, &NewFileDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &NewFileDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// NewFileDlg ��Ϣ�������


BOOL NewFileDlg::OnInitDialog()
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
	isfile = true;
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void NewFileDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	auto && manager = z::Manager::Instance();

	bool a = isfile;
	CString filename;

	m_newfile.GetWindowTextW(filename);
	std::string temp_path = (CStringA)filename;
	std::string path = temp_path;
	std::string pwd = (CStringA)c_pwd;

	if (filename.IsEmpty()) {
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
			
	}
	CDialogEx::OnOK();
}


void NewFileDlg::OnBnClickedRadio1()
{
	isfile = true;
}


void NewFileDlg::OnBnClickedRadio2()
{
	isfile = false;
}
