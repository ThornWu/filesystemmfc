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
}user;//�û��������

// CLogin �Ի���

class CLogin : public CDialogEx
{
	DECLARE_DYNAMIC(CLogin)

public:
	CLogin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLogin();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = Login };
#endif
private:
	FILE *fp;
	char temp[MAX_CHAR_SIZE] = "";//��ʱ�����ַ������������ִ�



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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









