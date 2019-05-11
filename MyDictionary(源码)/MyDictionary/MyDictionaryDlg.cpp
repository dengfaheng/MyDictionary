
// MyDictionaryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyDictionary.h"
#include "MyDictionaryDlg.h"
#include "afxdialogex.h"
#include <afxpriv.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



char *key;
char content[2048] = { 0 };
int flag = 0;
struct dict *p;
int count; //单词个数



// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
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

CAboutDlg::CAboutDlg() : CDialog(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CMyDictionaryDlg 对话框



CMyDictionaryDlg::CMyDictionaryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MYDICTIONARY_DIALOG, pParent)
	, InputWord(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMyDictionaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT, InputWord);
}

BEGIN_MESSAGE_MAP(CMyDictionaryDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMyDictionaryDlg::OnBnClickedSearch)
	ON_BN_CLICKED(IDCANCEL, &CMyDictionaryDlg::OnBnClickedQuit)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDictionaryDlg::OnBnClickedCopy)
END_MESSAGE_MAP()


// CMyDictionaryDlg 消息处理程序

BOOL CMyDictionaryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	char pathbuf[1024];
	GetCurrentDirectory(sizeof(pathbuf), pathbuf);
	strcat(pathbuf,"\\dict.txt");
	//::MessageBoxA(NULL, pathbuf, "w", MB_OK);
	count = open_dict(&p, pathbuf);



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyDictionaryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyDictionaryDlg::OnPaint()
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
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyDictionaryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void format_string(char *str)//去掉字符串结尾的回车，空格字符
{
	size_t i;
	for (i = strlen(str) - 1; i >= 0; i--)
	{
		if (str[i] != '\n' && str[i] != ' ' && str[i] != '\r')
		{
			str[i + 1] = '\0';
			break;
		}
	}
}

int get_dict_size(FILE *pfile)//得到字典文件中词条总数
{
	if (pfile == NULL)
		return 0;

	int n = 0;
	char buf[2048];
	while (!feof(pfile))
	{
		fgets(buf, sizeof(buf), pfile);
		fgets(buf, sizeof(buf), pfile);
		n++;//词条数加一
	}
	return n;
}

//打开字典文件，读取内容
int open_dict(struct dict **p, const char *dict_filename)
{
	FILE *pfile = fopen(dict_filename, "r");
	if (pfile == NULL)
	{
		MessageBoxA(NULL, "打开词典文件失败，请确认词典文件是否放在程序目录下！", "警告", MB_OK | MB_ICONWARNING);
	}

	int dict_size = get_dict_size(pfile);//得到词条总数
	if (dict_size == 0)
		return 0;

	*p = (struct dict *)malloc(sizeof(struct dict) * dict_size);//分配内存
	memset(*p, 0, sizeof(struct dict) * dict_size);//将内存初始化为0

	char buf[2048] = { 0 };
	size_t len = 0;
	int i = 0;
	fseek(pfile, 0L, SEEK_SET);//设置读取位置为字典文件开始
	while (!feof(pfile))//循环读取文件，直到文件末尾
	{
		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), pfile);//读取文件一行
		len = strlen(buf);//得到读取到字符串长度
		if (len > 0)
		{
			(*p)[i].key = (char *)malloc(len);//根据字符串长度分配内存
			memset((*p)[i].key, 0, len);
			format_string(buf);//去掉字符串结尾的空格和回车
			strcpy((*p)[i].key, &buf[1]);//将读取到的内容拷贝到key中
		}

		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), pfile);
		len = strlen(buf);
		if (len > 0)
		{
			(*p)[i].content = (char *)malloc(len);
			memset((*p)[i].content, 0, len);
			strcpy((*p)[i].content, &buf[6]);
		}
		i++;
	}
	fclose(pfile);//关闭字典文件

	return i;//返回读取到的字典词条数
}

//根据关键字key,在字典中查找内容
int search_dict(const struct dict *p, int size, const char *key, char *content)
{
	int i = 0;
	for (i = 0; i < size; i++)//遍历字典
	{
		if ((p[i].key == NULL) || (p[i].content == NULL))
			continue;

		if (strcmp(p[i].key, key) == 0)
		{
			strcpy(content, p[i].content);
			return 1;//找到符合条件记录，返回1
		}
	}
	return 0;//没有找到符合条件记录，返回0
}

//释放内存
void free_dict(struct dict *p, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)//循环释放key与content成员内存
	{
		if (p[i].key)
			free(p[i].key);
		if (p[i].content)
			free(p[i].content);
	}
	free(p);//释放p内存
}


void CMyDictionaryDlg::OnBnClickedSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	//MessageBoxA(NULL, "hello", "hello", MB_OK);

	UpdateData(TRUE);

	key = (char *)InputWord.GetBuffer(InputWord.GetLength());
	

	flag = search_dict(p, count, key, content);
	if (flag == 0)//没有查到
	{
		GetDlgItem(IDC_STATIC1)->SetWindowText("没有找到该单词，请确认是否输入正确！");
		
	}
	else
	{
		CString Str(content);
		
		GetDlgItem(IDC_STATIC1)->SetWindowText(Str);
	}

	//CDialog::OnOK();
}




void CMyDictionaryDlg::OnBnClickedQuit()
{
	// TODO: 在此添加控件通知处理程序代码
	int ret;
	ret = MessageBoxA("真的要退出吗？", "提示", MB_YESNO | MB_ICONQUESTION);
	if (ret == IDYES)
	{
		if (p != NULL)
		{
			free_dict(p, count);
		}
		PostQuitMessage(0);
	}


	

	//CDialog::OnCancel();
}


void CMyDictionaryDlg::OnBnClickedCopy()
{
	// TODO: 在此添加控件通知处理程序代码

	CString copyStr(content);

	if (OpenClipboard())
	{
		HGLOBAL clipbuffer;
		char* buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, copyStr.GetLength() + 1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, (LPSTR)(LPCTSTR)copyStr);
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT, clipbuffer);
		CloseClipboard();
	}


}
