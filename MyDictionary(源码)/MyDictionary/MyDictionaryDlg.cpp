
// MyDictionaryDlg.cpp : ʵ���ļ�
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
int count; //���ʸ���



// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CMyDictionaryDlg �Ի���



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


// CMyDictionaryDlg ��Ϣ�������

BOOL CMyDictionaryDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	char pathbuf[1024];
	GetCurrentDirectory(sizeof(pathbuf), pathbuf);
	strcat(pathbuf,"\\dict.txt");
	//::MessageBoxA(NULL, pathbuf, "w", MB_OK);
	count = open_dict(&p, pathbuf);



	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMyDictionaryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMyDictionaryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void format_string(char *str)//ȥ���ַ�����β�Ļس����ո��ַ�
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

int get_dict_size(FILE *pfile)//�õ��ֵ��ļ��д�������
{
	if (pfile == NULL)
		return 0;

	int n = 0;
	char buf[2048];
	while (!feof(pfile))
	{
		fgets(buf, sizeof(buf), pfile);
		fgets(buf, sizeof(buf), pfile);
		n++;//��������һ
	}
	return n;
}

//���ֵ��ļ�����ȡ����
int open_dict(struct dict **p, const char *dict_filename)
{
	FILE *pfile = fopen(dict_filename, "r");
	if (pfile == NULL)
	{
		MessageBoxA(NULL, "�򿪴ʵ��ļ�ʧ�ܣ���ȷ�ϴʵ��ļ��Ƿ���ڳ���Ŀ¼�£�", "����", MB_OK | MB_ICONWARNING);
	}

	int dict_size = get_dict_size(pfile);//�õ���������
	if (dict_size == 0)
		return 0;

	*p = (struct dict *)malloc(sizeof(struct dict) * dict_size);//�����ڴ�
	memset(*p, 0, sizeof(struct dict) * dict_size);//���ڴ��ʼ��Ϊ0

	char buf[2048] = { 0 };
	size_t len = 0;
	int i = 0;
	fseek(pfile, 0L, SEEK_SET);//���ö�ȡλ��Ϊ�ֵ��ļ���ʼ
	while (!feof(pfile))//ѭ����ȡ�ļ���ֱ���ļ�ĩβ
	{
		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), pfile);//��ȡ�ļ�һ��
		len = strlen(buf);//�õ���ȡ���ַ�������
		if (len > 0)
		{
			(*p)[i].key = (char *)malloc(len);//�����ַ������ȷ����ڴ�
			memset((*p)[i].key, 0, len);
			format_string(buf);//ȥ���ַ�����β�Ŀո�ͻس�
			strcpy((*p)[i].key, &buf[1]);//����ȡ�������ݿ�����key��
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
	fclose(pfile);//�ر��ֵ��ļ�

	return i;//���ض�ȡ�����ֵ������
}

//���ݹؼ���key,���ֵ��в�������
int search_dict(const struct dict *p, int size, const char *key, char *content)
{
	int i = 0;
	for (i = 0; i < size; i++)//�����ֵ�
	{
		if ((p[i].key == NULL) || (p[i].content == NULL))
			continue;

		if (strcmp(p[i].key, key) == 0)
		{
			strcpy(content, p[i].content);
			return 1;//�ҵ�����������¼������1
		}
	}
	return 0;//û���ҵ�����������¼������0
}

//�ͷ��ڴ�
void free_dict(struct dict *p, int size)
{
	int i = 0;
	for (i = 0; i < size; i++)//ѭ���ͷ�key��content��Ա�ڴ�
	{
		if (p[i].key)
			free(p[i].key);
		if (p[i].content)
			free(p[i].content);
	}
	free(p);//�ͷ�p�ڴ�
}


void CMyDictionaryDlg::OnBnClickedSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//MessageBoxA(NULL, "hello", "hello", MB_OK);

	UpdateData(TRUE);

	key = (char *)InputWord.GetBuffer(InputWord.GetLength());
	

	flag = search_dict(p, count, key, content);
	if (flag == 0)//û�в鵽
	{
		GetDlgItem(IDC_STATIC1)->SetWindowText("û���ҵ��õ��ʣ���ȷ���Ƿ�������ȷ��");
		
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int ret;
	ret = MessageBoxA("���Ҫ�˳���", "��ʾ", MB_YESNO | MB_ICONQUESTION);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

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
