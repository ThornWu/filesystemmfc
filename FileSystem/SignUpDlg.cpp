// SignUpDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FileSystem.h"
#include "SignUpDlg.h"
#include "afxdialogex.h"


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
}


BEGIN_MESSAGE_MAP(SignUpDlg, CDialogEx)
END_MESSAGE_MAP()


// SignUpDlg 消息处理程序
