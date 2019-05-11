
// MyDictionaryDlg.h : ͷ�ļ�
//

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



struct dict
{
	char *key;
	char *content;
};



void format_string(char *str);
int get_dict_size(FILE *pfile);
int open_dict(struct dict **p, const char *dict_filename);
int search_dict(const struct dict *p, int size, const char *key, char *content);
void free_dict(struct dict *p, int size);




// CMyDictionaryDlg �Ի���
class CMyDictionaryDlg : public CDialog
{
// ����
public:
	CMyDictionaryDlg(CWnd* pParent = NULL);	// ��׼���캯��


// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYDICTIONARY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:




	afx_msg void OnBnClickedSearch();

	afx_msg void OnBnClickedQuit();
	// ���뵥��
	CString InputWord;
	afx_msg void OnBnClickedCopy();
};


