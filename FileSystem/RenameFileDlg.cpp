// RenameFileDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileSystem.h"
#include "RenameFileDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include "file.h"
#include "manager.h"
#include <regex>


// RenameFileDlg 对话框

IMPLEMENT_DYNAMIC(RenameFileDlg, CDialogEx)

RenameFileDlg::RenameFileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

RenameFileDlg::~RenameFileDlg()
{
}

void RenameFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT5, m_rename);
}


BEGIN_MESSAGE_MAP(RenameFileDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &RenameFileDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// RenameFileDlg 消息处理程序


void RenameFileDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	auto && manager = z::Manager::Instance();
	CString filename;
	m_rename.GetWindowTextW(filename);
	if (filename.IsEmpty()) {
		MessageBox(_T("文件名称不能为空"));
		return;
	}

	else {
		std::string pwd = (CStringA)c_pwd;
		std::string oldfile = (CStringA)oldname;
		std::string newfile = (CStringA)filename;
		std::string path = (CStringA)filename;

		if (!z::Path::isFullPath(oldfile)) path = pwd + "/" + oldfile;

		if (!manager.rename(path, newfile)) {
			MessageBox(_T("重命名失败"));
		}
		else {
			MessageBox(_T("修改成功"));
		}
	}
	CDialogEx::OnOK();
}
