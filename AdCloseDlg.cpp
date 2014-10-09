
// AdCloseDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "AdClose.h"
#include "AdCloseDlg.h"

#include "windowsx.h"
#include "psapi.h"
#pragma comment(lib, "psapi.lib")
#define WM_AC_NID	WM_USER+235

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CAdCloseDlg 对话框


CAdCloseDlg::CAdCloseDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdCloseDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CAdCloseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BEGIN_MESSAGE_MAP(CAdCloseDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_WORK, &CAdCloseDlg::OnBnClickedWork)
	ON_WM_TIMER()
	ON_MESSAGE(WM_AC_NID, OnAcNid)
	ON_COMMAND(ID_TRAY_CLOSE, OnTrayClose)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAdCloseDlg 消息处理程序

BOOL CAdCloseDlg::OnInitDialog()
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	this->SetTimer(12123, 10, NULL);

	::CreateMutex(NULL,TRUE,_T("adclose_mt"));
	if(GetLastError()==ERROR_ALREADY_EXISTS)
	{
		//dup
		exit(0);
	}

	m_list.InsertColumn(0, "窗口类名", LVCFMT_LEFT, 150);
	m_list.InsertColumn(0, "窗口标题", LVCFMT_LEFT, 150);
	m_list.InsertColumn(0, "时间", LVCFMT_LEFT, 150);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CAdCloseDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CAdCloseDlg::OnPaint()
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
HCURSOR CAdCloseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

HWND g_lastWnd = NULL;

BOOL CALLBACK lpEnumFunc(HWND hwnd, LPARAM lParam)
{
	DWORD style = GetWindowStyle(hwnd);
	DWORD styleex = GetWindowExStyle(hwnd);

	int width = GetSystemMetrics ( SM_CXSCREEN );
	int height= GetSystemMetrics ( SM_CYSCREEN );

	CRect rect;
	GetWindowRect(hwnd, &rect);
	
	TCHAR text[1000];
	GetWindowText(hwnd, text, ARRAYSIZE(text));
	CString stext = text;

	char name[1000]="";
	GetClassNameA(hwnd, name, ARRAYSIZE(name));
	CString sname = name;


	if (rect.Height() < 400 && rect.Width() < 400 && rect.Height() > 100)
	{
		if (abs(width - rect.right) < 100 && abs(height - rect.bottom) < 100)
		{
			if ((style&WS_VISIBLE) && (style&WS_POPUP))
			{
				_strlwr_s(name);
				_strlwr_s(text);
				if (strstr(text, "menu") == NULL)
				{
					if (strstr(name, "tx") != NULL || strstr(name, "xl") != NULL)
					{
						::PostMessage(hwnd, WM_CLOSE, 0, 0);
						::PostMessage(hwnd, WM_DESTROY, 0, 0);

						//
						if (hwnd != g_lastWnd)
						{
							CAdCloseDlg* dlg = (CAdCloseDlg*)AfxGetMainWnd();
							int count = dlg->m_list.GetItemCount();
							CTime tm = CTime::GetCurrentTime();
							CString ss;
							ss.Format("%d/%02d/%02d %02d:%02d:%02d", tm.GetYear(), tm.GetMonth(), tm.GetDay(), tm.GetHour(), tm.GetMinute(), tm.GetSecond());
							int idx = dlg->m_list.InsertItem(count, ss);
							dlg->m_list.SetItemText(idx, 1, stext);
							dlg->m_list.SetItemText(idx, 2, sname);
							dlg->m_list.EnsureVisible(idx, FALSE);

							dlg->SetDlgItemInt(IDC_STATIC_COUNT, count+1);

							g_lastWnd = hwnd;
						}
					}
				}
			}
		}
	}

	return 1;
}

void CAdCloseDlg::OnBnClickedWork()
{
	// TODO: 在此添加控件通知处理程序代码
	//EnumWindows(lpEnumFunc, NULL);
	ToTray();
}

bool tray = false;

void CAdCloseDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	EnumWindows(lpEnumFunc, NULL);

	if (!tray)
	{
		ToTray();
		tray = true;
	}

	CDialog::OnTimer(nIDEvent);
}

void CAdCloseDlg::ToTray() 
{
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP; 
	nid.uCallbackMessage = WM_AC_NID;
	nid.hIcon=LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME)); 
	_tcscpy_s(nid.szTip,sizeof(nid.szTip)+1,_T("AdClose"));
	Shell_NotifyIcon(NIM_ADD,&nid);
	ShowWindow(SW_HIDE);
}

LRESULT CAdCloseDlg::OnAcNid(WPARAM wp, LPARAM lp)
{
	if(wp!=IDR_MAINFRAME) return 1;

	switch(lp) 
	{ 
	case WM_RBUTTONUP://右键
		{ 
			POINT point; 
			::GetCursorPos(&point);
			CMenu menu; 
			VERIFY(menu.LoadMenu(IDR_MENU_TRAY)); 
			CMenu *pMenu = (CMenu *)(menu.GetSubMenu(0)); 
			ASSERT(pMenu != NULL);

			::SetForegroundWindow(m_hWnd); 
			pMenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, this);

			::PostMessage(m_hWnd, WM_NULL, 0, 0);
		} 
		break; 
	case WM_LBUTTONDBLCLK://双击左键的处理 
		{ 
			this->ShowWindow(SW_SHOW);
			BringWindowToTop();
		} 
		break; 
	case WM_ACTIVATE:
		{
			if(LOWORD(wp)!=WA_INACTIVE)
			{
				
			}
		}
		break;
	} 
	return 0;
}

BOOL g_quit = FALSE;

void CAdCloseDlg::OnTrayClose()
{
	// TODO: 在此添加命令处理程序代码

	NOTIFYICONDATA oIconData = {0};
	oIconData.cbSize = sizeof(NOTIFYICONDATA);
	oIconData.hWnd = m_hWnd;
	oIconData.uID = IDR_MAINFRAME;
	BOOL bRet = Shell_NotifyIcon(NIM_DELETE, &oIconData);

	g_quit = TRUE;
	PostMessage(WM_CLOSE);
}

void CAdCloseDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (!g_quit)
	{
		ToTray();
	}
	else
	{
		CDialog::OnClose();
	}
}
