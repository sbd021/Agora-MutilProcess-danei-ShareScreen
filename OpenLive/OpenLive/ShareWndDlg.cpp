// ShareWndDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OpenLive.h"
#include "ShareWndDlg.h"
#include "afxdialogex.h"


// CShareWndDlg dialog

IMPLEMENT_DYNAMIC(CShareWndDlg, CDialogEx)

CShareWndDlg::CShareWndDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShareWndDlg::IDD, pParent)
{
	m_brHead.CreateSolidBrush(RGB(0, 160, 239));
	SetBackgroundColor(RGB(0xFF, 0xFF, 0xFF));
}

CShareWndDlg::~CShareWndDlg()
{
	m_brHead.DeleteObject();
}

void CShareWndDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTNMIN_VIDEO, m_btnMin);
	DDX_Control(pDX, IDC_BTNRSTO_VIDEO, m_btnRst);
	DDX_Control(pDX, IDC_BTNCLOSE_VIDEO, m_btnClose);
}


BEGIN_MESSAGE_MAP(CShareWndDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTNMIN_VIDEO, &CShareWndDlg::OnBnClickedBtnmin)
	ON_BN_CLICKED(IDC_BTNCLOSE_VIDEO, &CShareWndDlg::OnBnClickedBtnclose)
	ON_BN_CLICKED(IDC_BTNENDCALL_VIDEO, &CShareWndDlg::OnBnClickedBtnclose)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()


// CShareWndDlg message handlers


void CShareWndDlg::InitCtrls()
{
	CRect	rcClient;
	int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);

	GetClientRect(&rcClient);


//	m_btnMin.MoveWindow(rcClient.Width() - 72, 1, 22, 22, TRUE);
//	m_btnRst.MoveWindow(rcClient.Width() - 48, 1, 22, 22, TRUE);
	m_btnClose.MoveWindow(rcClient.Width() - 24, 1, 22, 22, TRUE);
	m_btnMin.ShowWindow(SW_HIDE);
	m_btnRst.ShowWindow(SW_HIDE);

//	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0x00, 0xFF));
//	m_btnRst.SetBackImage(IDB_BTNMAX, RGB(0xFF, 0x00, 0xFF));
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0xFF, 0x00, 0xFF));
}



void CShareWndDlg::DrawHead(CDC *pDC)
{
	CRect rcTitle;
	CString strTitle;

	GetWindowText(strTitle);
	GetClientRect(&rcTitle);
	CBrush *lpDefaultBrush = pDC->SelectObject(&m_brHead);

	rcTitle.bottom = 24;
	pDC->FillRect(&rcTitle, &m_brHead);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	pDC->DrawText(strTitle, &rcTitle, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	pDC->SelectObject(lpDefaultBrush);
}

void CShareWndDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()
	
	CRect	rcText;
	CRect	rcClient;
	LPCTSTR lpString = NULL;
	GetClientRect(&rcClient);
	dc.SetBkColor(RGB(0x00, 0x9E, 0xEB));
	dc.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
	lpString = _T("Teacher Screen");//LANG_STR("IDS_TITLE");
	dc.TextOut(12, 3, lpString, _tcslen(lpString));
	DrawHead(&dc);
}

BOOL CShareWndDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	InitCtrls();

	GetClientRect(&m_rcVideoArea);
	m_rcVideoArea.top += 24;
	m_wndTeacherScreenShare.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, m_rcVideoArea/*CRect(m_rcVideoArea.left, 0, 1, 1)*/, this, IDC_BASEWND_VIDEO + 4);
	m_wndTeacherScreenShare.SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
	m_wndTeacherScreenShare.SetFaceColor(RGB(0x58, 0x58, 0x58));

	m_wndTeacherScreenShare.ShowWindow(SW_SHOW);
	//m_wndTeacherScreenShare.SetUID(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShareWndDlg::SetVideoInfo(int fps, int w, int h, int bitrate)
{
	m_wndTeacherScreenShare.SetBitrateInfo(bitrate);
	m_wndTeacherScreenShare.SetVideoResolution(w, h);
	m_wndTeacherScreenShare.SetFrameRateInfo(fps);
}

void CShareWndDlg::OnBnClickedBtnmin()
{
	ShowWindow(SW_MINIMIZE);
}

void CShareWndDlg::OnBnClickedBtnclose()
{
	CDialogEx::OnOK();
}

void CShareWndDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	if (m_btnClose.GetSafeHwnd() != NULL)
		m_btnClose.MoveWindow(cx - 24, 0, 24, 24, TRUE);

	GetClientRect(&m_rcVideoArea);
	m_rcVideoArea.top += 24;

	if (m_wndTeacherScreenShare.GetSafeHwnd() != NULL)
	    m_wndTeacherScreenShare.MoveWindow(m_rcVideoArea);
}


LRESULT CShareWndDlg::OnNcHitTest(CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
		lResult = HTCAPTION;

	return lResult;
}
