#pragma once
#include "afxwin.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <vector>
#define MAX_CHAR_SIZE 1000
typedef struct {
	CString name;
	CString pass;
}user;//用户结点声明

// CLogin 对话框

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogin();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = Login };
#endif
private:
	FILE *fp;
	char temp[MAX_CHAR_SIZE] = "";//临时保存字符（串）和数字串



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_user;
	CString m_password;
	afx_msg void OnBnClickedYes();
	afx_msg void OnBnClickedCancel();
	char sortChar(char ch);
	void stringType();
	void resetWord();
};









