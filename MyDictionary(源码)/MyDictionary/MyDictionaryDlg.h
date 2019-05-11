
// MyDictionaryDlg.h : 头文件
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




// CMyDictionaryDlg 对话框
class CMyDictionaryDlg : public CDialog
{
// 构造
public:
	CMyDictionaryDlg(CWnd* pParent = NULL);	// 标准构造函数


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYDICTIONARY_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()


public:




	afx_msg void OnBnClickedSearch();

	afx_msg void OnBnClickedQuit();
	// 输入单词
	CString InputWord;
	afx_msg void OnBnClickedCopy();
};


