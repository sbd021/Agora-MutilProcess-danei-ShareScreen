// VideoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OpenLive.h"
#include "VideoDlg.h"
#include "afxdialogex.h"
#include "AGEventDef.h"
#include "commonFun.h"

// CVideoDlg �Ի���

IMPLEMENT_DYNAMIC(CVideoDlg, CDialogEx)

CVideoDlg::CVideoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CVideoDlg::IDD, pParent)
{
	m_brHead.CreateSolidBrush(RGB(0, 160, 239));
	SetBackgroundColor(RGB(0x26, 0x26, 0x26));
	m_bLastStat = FALSE;
	m_nScreenMode = SCREEN_VIDEO1;
	m_bFullScreen = FALSE;
	m_nTimeCounter = 0;
	m_lpBigShowed = NULL;

}

CVideoDlg::~CVideoDlg()
{
	m_brHead.DeleteObject();
}

void CVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

//	DDX_Control(pDX, IDC_CBXROLE_VIDEO, m_cbxRole);
	DDX_Control(pDX, IDC_BTNMIN_VIDEO, m_btnMin);
	DDX_Control(pDX, IDC_BTNRSTO_VIDEO, m_btnRst);
	DDX_Control(pDX, IDC_BTNCLOSE_VIDEO, m_btnClose);
}


BEGIN_MESSAGE_MAP(CVideoDlg, CDialogEx)
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_NCHITTEST()

	ON_MESSAGE(WM_SHOWMODECHANGED, &CVideoDlg::OnShowModeChanged)
	ON_MESSAGE(WM_SHOWBIG, &CVideoDlg::OnShowBig)

	ON_MESSAGE(WM_MSGID(EID_JOINCHANNEL_SUCCESS), &CVideoDlg::OnEIDJoinChannelSuccess)
	ON_MESSAGE(WM_MSGID(EID_REJOINCHANNEL_SUCCESS), &CVideoDlg::OnEIDReJoinChannelSuccess)
	ON_MESSAGE(WM_MSGID(EID_FIRST_LOCAL_VIDEO_FRAME), &CVideoDlg::OnEIDFirstLocalFrame)

	ON_MESSAGE(WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED), &CVideoDlg::OnEIDFirstRemoteFrameDecoded)
	ON_MESSAGE(WM_MSGID(EID_USER_JOINED),&CVideoDlg::OnEIDUserJoined)
	ON_MESSAGE(WM_MSGID(EID_USER_OFFLINE), &CVideoDlg::OnEIDUserOffline)
	
	ON_MESSAGE(WM_MSGID(EID_REMOTE_VIDEO_STAT), &CVideoDlg::OnRemoteVideoStat)

	ON_MESSAGE(WM_MSGID(EID_START_RCDSRV), &CVideoDlg::OnStartRecordingService)
	ON_MESSAGE(WM_MSGID(EID_STOP_RCDSRV), &CVideoDlg::OnStopRecordingService)
	ON_MESSAGE(WM_WINDOWSHARE,&CVideoDlg::OnWindowsShare)
	
    ON_BN_CLICKED(IDC_BTNMIN_VIDEO, &CVideoDlg::OnBnClickedBtnmin)
	ON_BN_CLICKED(IDC_BTNCLOSE_VIDEO, &CVideoDlg::OnBnClickedBtnclose)
	ON_BN_CLICKED(IDC_BTNRSTO_VIDEO, &CVideoDlg::OnBnClickedBtnrest)
	ON_BN_CLICKED(IDC_BTNENDCALL_VIDEO, &CVideoDlg::OnBnClickedBtnclose)

	ON_BN_CLICKED(IDC_BTNTIP_VIDEO, &CVideoDlg::OnBnClickedBtntip)
	ON_BN_CLICKED(IDC_BTNSETUP_VIDEO, &CVideoDlg::OnBnClickedBtnsetup)
	ON_BN_CLICKED(IDC_BTNMODE_VIDEO, &CVideoDlg::OnBnClickedBtnmode)
	ON_BN_CLICKED(IDC_BTNAUDIO_VIDEO, &CVideoDlg::OnBnClickedBtnaudio)
	ON_BN_CLICKED(IDC_BTNMESSAGE_VIDEO, &CVideoDlg::OnBnClickedBtnmessage)
	ON_BN_CLICKED(IDC_BTNSCRCAP_VIDEO, &CVideoDlg::OnBnClickedBtnScreenCapture)
	ON_BN_CLICKED(IDC_BTNMORE_VIDEO, &CVideoDlg::OnBnClickedBtnMore)
	ON_BN_CLICKED(IDC_BTNSCR_VIDEO, &CVideoDlg::OnBnClickedBtnfullscr)

	ON_BN_CLICKED(ID_SCRSHARE_DESKTOPSHARE, &CVideoDlg::OnBnClickedScreenshare)
	ON_BN_CLICKED(ID_SCRSHARE_WINDOWSHARE, &CVideoDlg::OnBnClickedWindowshare)
	
	ON_BN_CLICKED(ID_IDR_DEVICE, &CVideoDlg::OnBnClickedBtnsetup)
	ON_BN_CLICKED(ID_IDR_SEIPUSH, &CVideoDlg::OnBnClickedBtSEIPush)

	ON_CBN_SELCHANGE(IDC_CBXROLE_VIDEO, &CVideoDlg::OnCbnSelchangeCmbRole)

	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CVideoDlg ��Ϣ�������


void CVideoDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO:  �ڴ˴������Ϣ����������
	if (m_btnMin.GetSafeHwnd() != NULL)
		m_btnMin.MoveWindow(cx - 72, 0, 24, 24, TRUE);
	if (m_btnRst.GetSafeHwnd() != NULL)
		m_btnRst.MoveWindow(cx - 48, 0, 24, 24, TRUE);
	if (m_btnClose.GetSafeHwnd() != NULL)
		m_btnClose.MoveWindow(cx - 24, 0, 24, 24, TRUE);

	GetClientRect(&m_rcVideoArea);
	m_rcVideoArea.top += 24;
	m_rcVideoArea.bottom -= 72;

	// 2�ˣ� ���Ͻ��ӻ�������
	m_rcChildVideoArea.top = m_rcVideoArea.top + 10;
	m_rcChildVideoArea.bottom = m_rcChildVideoArea.top + 144;
	m_rcChildVideoArea.right = m_rcVideoArea.right - 14;
	m_rcChildVideoArea.left = m_rcChildVideoArea.right - 192;

	if (::IsWindow(m_dlgChat.GetSafeHwnd()) && m_dlgChat.IsWindowVisible()) {
		CRect   rcChatArea;

		rcChatArea.CopyRect(&m_rcVideoArea);
		rcChatArea.top = rcChatArea.bottom - 150;
		ClientToScreen(&rcChatArea);
		m_dlgChat.MoveWindow(&rcChatArea);
	}

	if (m_wndLocal.GetSafeHwnd() == NULL)
		return;

	m_wndLocal.MoveWindow(&m_rcVideoArea, FALSE);
	AdjustButtonsNormal(cx, cy);
	Invalidate(TRUE);
}

void CVideoDlg::ShowButtonsNormal()
{
	CRect	rcClient;
	int nShowMode = SW_HIDE;

	if (!m_bFullScreen)
		nShowMode = SW_SHOW;

	m_btnMode.ShowWindow(nShowMode);
	m_btnMore.ShowWindow(nShowMode);

	m_btnAudio.SetBackImage(IDB_BTNMAUDIO_VIDEO, RGB(0x26, 0x26, 0x26));
	m_btnAudio.ShowWindow(nShowMode);
	m_btnEndCall.SetBackImage(IDB_BTNENDCALL_VIDEO, RGB(0x26, 0x26, 0x26));
	m_btnEndCall.ShowWindow(nShowMode);
	m_btnShow.ShowWindow(nShowMode);
	m_btnTip.ShowWindow(nShowMode);
	m_btnScrCap.ShowWindow(nShowMode);

	GetClientRect(&rcClient);
	AdjustButtonsNormal(rcClient.Width(), rcClient.Height());
}


void CVideoDlg::AdjustButtonsNormal(int cx, int cy)
{
	if (m_btnMode.GetSafeHwnd() != NULL)
		m_btnMode.MoveWindow(cx / 2 - 216, cy - 60, 48, 48, TRUE);
	if (m_btnAudio.GetSafeHwnd() != NULL)
		m_btnAudio.MoveWindow(cx / 2 - 120, cy - 60, 48, 48, TRUE);
	if (m_btnEndCall.GetSafeHwnd() != NULL)
		m_btnEndCall.MoveWindow(cx / 2 - 24, cy - 60, 48, 48, TRUE);

	if (m_btnScrCap.GetSafeHwnd() != NULL)
		m_btnScrCap.MoveWindow(cx / 2 + 72, cy - 60, 48, 48, TRUE);

	if (m_btnMore.GetSafeHwnd() != NULL)
		m_btnMore.MoveWindow(cx / 2 + 168, cy - 60, 48, 48, TRUE);

	if (m_cbxRole.GetSafeHwnd() != NULL)
		m_cbxRole.MoveWindow(cx / 2 - 370, cy - 50, 120, 42, TRUE);;

	if (m_btnShow.GetSafeHwnd() != NULL)
		m_btnShow.MoveWindow(cx - 126, cy - 48, 24, 24, TRUE);
	if (m_btnTip.GetSafeHwnd() != NULL)
		m_btnTip.MoveWindow(cx - 54, cy - 48, 24, 24, TRUE);
}

void CVideoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
		if (m_bFullScreen) {
		m_nTimeCounter = 5;
		ShowControlButton(TRUE);
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

BOOL CVideoDlg::NeedShowSizeBox(CPoint point)
{
	CRect rcClient;

	GetClientRect(&rcClient);

	rcClient.left += 5;
	rcClient.right -= 5;
	rcClient.top += 5;
	rcClient.bottom -= 5;
	
	if (rcClient.PtInRect(point) || IsZoomed())
		return FALSE;

	return TRUE;
}

void CVideoDlg::EnableSize(BOOL bEnable)
{
	LONG lStyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);

	if (bEnable)
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, lStyle | WS_SIZEBOX);
	else
		::SetWindowLong(GetSafeHwnd(), GWL_STYLE, lStyle&(~WS_SIZEBOX));

	CRect rcClient;

	if (bEnable)
		GetWindowRect(&rcClient);
	else
		GetClientRect(&rcClient);

	int cx = rcClient.Width();
	int cy = rcClient.Height();

	if (m_bLastStat != bEnable) {
		m_bLastStat = bEnable;
		if (m_btnMin.GetSafeHwnd() != NULL)
			m_btnMin.MoveWindow(cx - 72, 0, 24, 24, TRUE);
		if (m_btnRst.GetSafeHwnd() != NULL)
			m_btnRst.MoveWindow(cx - 48, 0, 24, 24, TRUE);
		if (m_btnClose.GetSafeHwnd() != NULL)
			m_btnClose.MoveWindow(cx - 24, 0, 24, 24, TRUE);

		if (m_btnTip.GetSafeHwnd() != NULL)
			m_btnTip.MoveWindow(cx / 2 - 372, cy - 84, 72, 72, TRUE);

		if (m_btnMode.GetSafeHwnd() != NULL)
			m_btnMode.MoveWindow(cx / 2 - 96, cy - 84, 72, 72, TRUE);
		if (m_btnAudio.GetSafeHwnd() != NULL)
			m_btnAudio.MoveWindow(cx / 2 + 24, cy - 84, 72, 72, TRUE);
		if (m_btnShow.GetSafeHwnd() != NULL)
			m_btnShow.MoveWindow(cx / 2 + 144, cy - 84, 72, 72, TRUE);
		if (m_btnEndCall.GetSafeHwnd() != NULL)
			m_btnEndCall.MoveWindow(cx - 120, cy - 84, 72, 72, TRUE);

		Invalidate(FALSE);
	}
}

void CVideoDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	DrawHead(&dc);
}

LRESULT CVideoDlg::OnNcHitTest(CPoint point)
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	LRESULT lResult = CDialogEx::OnNcHitTest(point);
	if (lResult == HTCLIENT && ::GetAsyncKeyState(MK_LBUTTON) < 0)
		lResult = HTCAPTION;

	return lResult;
}

void CVideoDlg::OnBnClickedBtnmin()
{
	ShowWindow(SW_MINIMIZE);
}

void CVideoDlg::OnBnClickedBtnclose()
{
	COPYDATASTRUCT pCopyData;
	if (TRUE) {
		pCopyData.dwData = ShareType_Close;
		pCopyData.lpData = NULL;
		pCopyData.cbData = 0;
	}
	::PostMessage(m_WndScreenShare, WM_MSGID(EID_SCREENSHARE_CLOSE), NULL, NULL);
	m_WndScreenShare = nullptr;
	m_HandleData.best_handle = nullptr;

	m_shareWndDlg.ShowWindow(SW_HIDE);
	GetParent()->SendMessage(WM_LEAVECHANNEL, 0, 0);

	m_listWndInfo.RemoveAll();
	m_dlgChat.ShowWindow(SW_HIDE);
	m_dlgDevice.ShowWindow(SW_HIDE);

	// unmute local video
	CAgoraObject::GetAgoraObject()->MuteLocalVideo(FALSE);
	m_btnMode.SetBackImage(IDB_BTNAUDIO_VIDEO);

	// unmute local audio
	CAgoraObject::GetAgoraObject()->MuteLocalAudio(FALSE);
	m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

	CAgoraObject::GetAgoraObject()->EnableScreenCapture(NULL, 0, NULL, FALSE);
	m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);

	m_dlgChat.ShowWindow(SW_HIDE);
	m_dlgChat.ClearHistory();

	CAgoraObject::GetAgoraObject()->RemoveAllSEIInfo();

	CDialogEx::OnOK();
}

void CVideoDlg::OnBnClickedBtnrest()
{
	if (IsZoomed()) {
		m_btnRst.SetBackImage(IDB_BTNMAX, RGB(0xFF, 0x00, 0xFF));
		ShowWindow(SW_RESTORE);
	}
	else{
		m_btnRst.SetBackImage(IDB_BTNRESTORE, RGB(0xFF, 0x00, 0xFF));
		ShowWindow(SW_MAXIMIZE);
	}

	CRect	rcClient;

	GetClientRect(&rcClient);
	m_rcVideoArea.CopyRect(&rcClient);
	m_rcVideoArea.top += 24;
	m_rcVideoArea.bottom -= 72;
	  
	m_rcChildVideoArea.top = m_rcVideoArea.top + 16;
	m_rcChildVideoArea.bottom = m_rcChildVideoArea.top + 144;
	m_rcChildVideoArea.left = m_rcVideoArea.right - 212;
	m_rcChildVideoArea.right = m_rcVideoArea.right - 20;

	int cx = rcClient.Width();
	int cy = rcClient.Height();
	m_wndLocal.MoveWindow(&m_rcVideoArea, FALSE);
	Invalidate(TRUE);
}


void CVideoDlg::OnBnClickedBtnfullscr()
{
	int nShowMode = 0;

	if (m_bFullScreen) {
		m_bFullScreen = FALSE;
		nShowMode = SW_SHOW;		
		ShowWindow(SW_RESTORE);

		GetClientRect(&m_rcVideoArea);
		m_rcVideoArea.top += 24;
		m_rcVideoArea.bottom -= 72;	
	}
	else{
		m_bFullScreen = TRUE;
		nShowMode = SW_HIDE;
		ShowWindow(SW_MAXIMIZE);

		GetClientRect(&m_rcVideoArea);
	}

	m_btnMin.ShowWindow(nShowMode);
	m_btnRst.ShowWindow(nShowMode);
	m_btnClose.ShowWindow(nShowMode);
	
	m_btnMode.ShowWindow(nShowMode);
	m_btnAudio.ShowWindow(nShowMode);
	m_btnEndCall.ShowWindow(nShowMode);
	
	m_btnMore.ShowWindow(nShowMode);

	m_btnShow.ShowWindow(nShowMode);
	m_btnTip.ShowWindow(nShowMode);

// 	switch (m_nScreenMode)
// 	{
// 	case SCREEN_VIDEO1:
// 		ShowVideo1();
// 		break;
// 	case SCREEN_VIDEO4:
// 		ShowVideo4();
// 		break;
// 	case SCREEN_VIDEOMULTI:
// 		ShowMulti();
// 		break;
// 	default:
// 		break;
// 	}

	Invalidate(TRUE);
}

void CVideoDlg::OnCbnSelchangeCmbRole()
{
	int nSel = m_cbxRole.GetCurSel();

	if (nSel == 0)
		CAgoraObject::GetAgoraObject()->SetClientRole(CLIENT_ROLE_TYPE::CLIENT_ROLE_BROADCASTER);
	else
		CAgoraObject::GetAgoraObject()->SetClientRole(CLIENT_ROLE_TYPE::CLIENT_ROLE_AUDIENCE);
}

void CVideoDlg::OnBnClickedScreenshare()
{
#if 0
	IRtcEngine *lpRtcEngine = CAgoraObject::GetEngine();

	CAgoraObject::GetAgoraObject()->EnableScreenCapture(::GetDesktopWindow(), 15, NULL, TRUE);
	m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_PUSH);

	m_wndLocal.Invalidate(TRUE);
	m_wndLocal.UpdateWindow();

	Invalidate(TRUE);
	UpdateWindow();
#else

	HWND hMarkWnd = ::GetDesktopWindow();
	if (::IsWindow(hMarkWnd)) {

		CAgoraObject::GetAgoraObject()->SetScreenCaptureEnable(TRUE);
		AGE_SCREENSHARE_START lpData;
		lpData.hWnd = hMarkWnd;
		PCOPYDATASTRUCT pCopyData = new COPYDATASTRUCT;
		if (TRUE){

			pCopyData->dwData = ShareType_Start;
			pCopyData->lpData = (PVOID)&lpData;
			pCopyData->cbData = sizeof(lpData);
			::SendMessage(m_WndScreenShare, WM_COPYDATA, WPARAM(m_hWnd), LPARAM(pCopyData));
		}
	}

#endif
}

void CVideoDlg::OnBnClickedWindowshare()
{
	CAgoraObject::GetAgoraObject()->SetScreenCaptureEnable(TRUE);
	m_dlgScreenCapture.RefreashWndInfo();
	m_dlgScreenCapture.SaveScreen(NULL);

	m_dlgScreenCapture.ShowWindow(SW_MAXIMIZE);
}

void CVideoDlg::ShowControlButton(BOOL bShow)
{
	int nShowMode = 0;
	if (bShow)
		nShowMode = SW_SHOW;
	else
		nShowMode = SW_HIDE;

	m_btnTip.ShowWindow(nShowMode);
	m_btnMode.ShowWindow(nShowMode);
	m_btnAudio.ShowWindow(nShowMode);
	m_btnShow.ShowWindow(nShowMode);
	m_btnEndCall.ShowWindow(nShowMode);
	m_btnMore.ShowWindow(nShowMode);
}

void CVideoDlg::CreateScreenShareMenu()
{
	CRect				rcButton;
	CMenu				Menu;
	MENUITEMINFO		mii;

	Menu.LoadMenu(IDR_MENU_SCRSHARE);

	CMenu *pMenuPopup = Menu.GetSubMenu(0);

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING;

	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_SCRSHARE");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_SCRSHARE")) + 1;
	pMenuPopup->SetMenuItemInfo(0, &mii, FALSE);

	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_WNDSHARE");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_WNDSHARE")) + 1;
	pMenuPopup->SetMenuItemInfo(1, &mii, FALSE);
	m_btnScrCap.GetWindowRect(&rcButton);

	int x = rcButton.left - 60;
	int y = rcButton.top - 50;

	pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, x, y, this);
	pMenuPopup->DestroyMenu();
}

void CVideoDlg::CreateMoreMenu()
{
	CRect			rcButton;
	CMenu			Menu;
	MENUITEMINFO	mii;
	UINT            nFlag = 0;
	int				nClientRole = CAgoraObject::GetAgoraObject()->GetClientRole();

	Menu.LoadMenu(IDR_MENU_MORE);

	CMenu *pMenuPopup = Menu.GetSubMenu(0);

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_STRING;

	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_MOREDEVICE");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_MOREDEVICE")) + 1;
	pMenuPopup->SetMenuItemInfo(0, &mii, FALSE);
	pMenuPopup->SetMenuItemBitmaps(ID_IDR_DEVICE, MF_BYCOMMAND, &m_bitMenuDevice, &m_bitMenuDevice);

	mii.dwTypeData = const_cast<LPTSTR>LANG_STR("IDS_VIDEO_MORESEI");
	mii.cch = _tcslen(LANG_STR("IDS_VIDEO_MORESEI")) + 1;
	pMenuPopup->SetMenuItemInfo(4, &mii, FALSE);
	pMenuPopup->SetMenuItemBitmaps(ID_IDR_SEIPUSH, MF_BYCOMMAND, &m_bitMenuSEI, &m_bitMenuSEI);

	m_btnMore.GetWindowRect(&rcButton);

	int x = rcButton.left - 60;
	int y = rcButton.top - 50;

	pMenuPopup->TrackPopupMenu(TPM_RIGHTBUTTON, x, y, this);
	pMenuPopup->DestroyMenu();
}

void CVideoDlg::OnBnClickedBtntip()
{
// 	BOOL bShowed = m_wndVideo[0].IsVideoInfoShowed();
// 
// 	for (int nIndex = 0; nIndex < 4; nIndex++)
// 		m_wndVideo[nIndex].ShowVideoInfo(!bShowed);
// 
// 	if (m_wndVideo[0].IsVideoInfoShowed())
// 		m_btnTip.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
// 	else
// 		m_btnTip.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
}

void CVideoDlg::OnBnClickedBtnScreenCapture()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();

	if (lpAgora->IsScreenCaptureEnabled()) {
#if 0
		lpAgora->EnableScreenCapture(NULL, 0, NULL, FALSE);
#else
		lpAgora->SetScreenCaptureEnable(FALSE);
		
		AGE_SCREENSHARE_START lpData;
		lpData.hWnd = m_WndScreenShare;
		PCOPYDATASTRUCT pCopyData = new COPYDATASTRUCT;
		if (TRUE){

			pCopyData->dwData = ShareType_Stop;
			pCopyData->lpData = (PVOID)&lpData;
			pCopyData->cbData = sizeof(lpData);
			::SendMessage(m_WndScreenShare, WM_COPYDATA, WPARAM(m_hWnd), LPARAM(pCopyData));
		}
#endif
		m_btnScrCap.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	}
	else
		CreateScreenShareMenu();


	Invalidate();
}

void CVideoDlg::OnBnClickedBtnMore()
{
	CreateMoreMenu();
}

void CVideoDlg::OnBnClickedBtnmessage()
{
	BOOL bIsShowing = m_dlgChat.IsWindowVisible();
	CRect   rcChatArea;

	rcChatArea.CopyRect(&m_rcVideoArea);
	rcChatArea.top = rcChatArea.bottom - 150;
	ClientToScreen(&rcChatArea);

	if (bIsShowing)
		m_dlgChat.ShowWindow(SW_HIDE);
	else {
		m_dlgChat.ShowWindow(SW_SHOW);
		m_dlgChat.MoveWindow(&rcChatArea, TRUE);
	}
}

void CVideoDlg::OnBnClickedBtnsetup()
{
	m_dlgDevice.ShowWindow(SW_SHOW);
	m_dlgDevice.CenterWindow();
}

void CVideoDlg::OnBnClickedBtSEIPush()
{
	m_dlgSEI.ShowWindow(SW_SHOW);
	m_dlgSEI.CenterWindow();
}

void CVideoDlg::OnBnClickedBtnmode()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();

	if (!lpAgora->IsLocalVideoMuted()) {
		lpAgora->MuteLocalVideo(TRUE);
		m_btnMode.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
	}
	else {
		lpAgora->MuteLocalVideo(FALSE);
		m_btnMode.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	}

	Invalidate();
}

void CVideoDlg::OnBnClickedBtnaudio()
{
	CAgoraObject *lpAgora = CAgoraObject::GetAgoraObject();

	if (lpAgora->IsLocalAudioMuted()) {
		lpAgora->MuteLocalAudio(FALSE);
		m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_NORMAL);
	}
	else {
		lpAgora->MuteLocalAudio(TRUE);
		m_btnAudio.SwitchButtonStatus(CAGButton::AGBTN_PUSH);
	}
}

BOOL CALLBACK EnumWindowsCallback(HWND handle, LPARAM lParam)
{
	handle_data& data = *(handle_data*)lParam;
	unsigned long process_id = 0;
	GetWindowThreadProcessId(handle, &process_id);
	char szbuf[MAX_PATH] = { '\0' };
	OutputDebugStringA(szbuf);
	if (data.process_id == process_id) {
		sprintf_s(szbuf, "!!!!!!!!!!!!!!!handle :%x, processId: %u\n", handle, process_id);
		OutputDebugStringA(szbuf);
		data.best_handle = handle;
		return FALSE;
	}

	return TRUE;
}

LRESULT CVideoDlg::OnEIDJoinChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	LPAGE_JOINCHANNEL_SUCCESS lpData = (LPAGE_JOINCHANNEL_SUCCESS)wParam;

	CAgoraObject *lpAgoraObject = CAgoraObject::GetAgoraObject();
	m_listWndInfo.RemoveAll();
	
	lpAgoraObject->SetSelfUID(lpData->uid);

	SEI_INFO seiInfo;
	if (lpAgoraObject->GetSEIInfo(0, &seiInfo))
		seiInfo.nUID = lpAgoraObject->GetSelfUID();

	lpAgoraObject->RemoveSEIInfo(0);
	lpAgoraObject->SetSEIInfo(lpAgoraObject->GetSelfUID(), &seiInfo);

	delete lpData; lpData = nullptr;

	if (CAgoraObject::GetAgoraObject()->GetClientRole() == CLIENT_ROLE_BROADCASTER)
	{
		//ScreenShare
		int nNum = 0;
		//closeProcess("ProcessScreenShare.exe", nNum);
		int dwProcessId = 0;
		dwProcessId = getProcessID("ProcessScreenShare.exe");
		if (0 >= dwProcessId)
			dwProcessId = openProcess("ProcessScreenShare.exe", "");
		m_HandleData.process_id = dwProcessId;
		do{
			EnumWindows(EnumWindowsCallback, (LPARAM)(&m_HandleData));
		} while (!m_HandleData.best_handle);
		m_WndScreenShare = m_HandleData.best_handle;

		AGE_SCREENSHARE_BASEINFO baseInfoTemp;
		if (TRUE) {

			CString strAppId = lpAgoraObject->GetAppID();
			baseInfoTemp.appid = cs2s(strAppId);
			CString strChannelName = lpAgoraObject->GetChanelName();
			baseInfoTemp.channelname = cs2s(strChannelName);
			UINT uid = lpAgoraObject->GetSelfUID();
			baseInfoTemp.uSubuID = uid + 1;
			baseInfoTemp.uMainuID = lpAgoraObject->GetSelfUID();

			lpAgoraObject->MuteRemoteAudio(uid + 1, TRUE);
			lpAgoraObject->MuteRemoteVideo(uid + 1, TRUE);

			char szbuf[24] = { '\0' };
			COPYDATASTRUCT cd;
			cd.dwData = ShareType_BaseInfo;
			cd.cbData = sizeof(baseInfoTemp);
			cd.lpData = (PVOID)&baseInfoTemp;

			::SendMessage(m_WndScreenShare, WM_COPYDATA, WPARAM(m_hWnd), LPARAM(&cd));
		}
	}
	return  0;
}

LRESULT CVideoDlg::OnEIDReJoinChannelSuccess(WPARAM wParam, LPARAM lParam)
{
	LPAGE_REJOINCHANNEL_SUCCESS lpData = (LPAGE_REJOINCHANNEL_SUCCESS)wParam;

	m_listWndInfo.RemoveAll();
	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnEIDFirstLocalFrame(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_LOCAL_VIDEO_FRAME lpData = (LPAGE_FIRST_LOCAL_VIDEO_FRAME)wParam;

	if (m_listWndInfo.GetCount() == 0)
		ShowVideo1();

	SEI_INFO seiInfo;

	CAgoraObject *lpAgoraObject = CAgoraObject::GetAgoraObject();

	memset(&seiInfo, 0, sizeof(SEI_INFO));

	seiInfo.nUID = lpAgoraObject->GetSelfUID();
	seiInfo.nWidth = lpData->width;
	seiInfo.nHeight = lpData->height;
	lpAgoraObject->SetSEIInfo(seiInfo.nUID, &seiInfo);

	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnEIDFirstRemoteFrameDecoded(WPARAM wParam, LPARAM lParam)
{
	return false;
}

LRESULT CVideoDlg::OnEIDUserJoined(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = (LPAGE_FIRST_REMOTE_VIDEO_DECODED)wParam;
	BOOL bFound = FALSE;
	SEI_INFO seiInfo;

	CAgoraObject* lpAgoraObject = CAgoraObject::GetAgoraObject();
	if (lpData->uid == lpAgoraObject->GetSelfUID() + 1){

		lpAgoraObject->MuteRemoteAudio(lpData->uid, TRUE);
		lpAgoraObject->MuteRemoteVideo(lpData->uid, TRUE);
		delete lpData; lpData = nullptr;
		return FALSE;
	}


	POSITION pos = m_listWndInfo.GetHeadPosition();
	while (pos != NULL) {
		AGVIDEO_WNDINFO &agvWndInfo = m_listWndInfo.GetNext(pos);
		if (agvWndInfo.nUID == lpData->uid) {
			bFound = TRUE;
			break;
		}
	}

	if (!bFound) {
		AGVIDEO_WNDINFO agvWndInfo;
		memset(&agvWndInfo, 0, sizeof(AGVIDEO_WNDINFO));
		agvWndInfo.nUID = lpData->uid;
		agvWndInfo.nWidth = lpData->width;
		agvWndInfo.nHeight = lpData->height;

		m_listWndInfo.AddTail(agvWndInfo);
	}

	RebindVideoWnd(lpData->uid);

	memset(&seiInfo, 0, sizeof(SEI_INFO));

	seiInfo.nUID = lpData->uid;
	seiInfo.nWidth = lpData->width;
	seiInfo.nHeight = lpData->height;
	CAgoraObject::GetAgoraObject()->SetSEIInfo(seiInfo.nUID, &seiInfo);

	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnEIDUserOffline(WPARAM wParam, LPARAM lParam)
{
	LPAGE_USER_OFFLINE lpData = (LPAGE_USER_OFFLINE)wParam;

	POSITION pos = m_listWndInfo.GetHeadPosition();
	while (pos != NULL){
		if (m_listWndInfo.GetAt(pos).nUID == lpData->uid) {
			m_listWndInfo.RemoveAt(pos);
			//RebindVideoWnd(lpData->uid);
			break;
		}

		m_listWndInfo.GetNext(pos);
	}

	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnEIDConnectionLost(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CVideoDlg::OnEIDVideoDeviceChanged(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CVideoDlg::OnRemoteVideoStat(WPARAM wParam, LPARAM lParam)
{
	LPAGE_REMOTE_VIDEO_STAT lpData = (LPAGE_REMOTE_VIDEO_STAT)wParam;

	POSITION posNext = m_listWndInfo.GetHeadPosition();

	if (lpData->uid == m_teacherUid){
		m_wndLocal.SetFrameRateInfo(lpData->receivedFrameRate);
		m_wndLocal.SetBitrateInfo(lpData->receivedBitrate);
		m_wndLocal.SetVideoResolution(lpData->width, lpData->height);
	}
	else if (lpData->uid == m_teacherUid + 1){
		m_shareWndDlg.SetVideoInfo(lpData->receivedFrameRate, lpData->width, lpData->height, lpData->receivedBitrate);
	}

	delete lpData;

	return 0;
}

LRESULT CVideoDlg::OnStartRecordingService(WPARAM wParam, LPARAM lParam)
{

	return 0;
}

LRESULT CVideoDlg::OnStopRecordingService(WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT CVideoDlg::OnWindowsShare(WPARAM wParam, LPARAM lParam)
{
#if 0
	HWND hMarkWnd = (HWND)wParam;
	if (::IsWindow(hMarkWnd)) {

		CAgoraObject::GetAgoraObject()->EnableScreenCapture(hMarkWnd);
	}

#else

	HWND hMarkWnd = (HWND)wParam;
	if(::IsWindow(hMarkWnd)) {

		AGE_SCREENSHARE_START StartTemp;
		StartTemp.hWnd = hMarkWnd;
		PCOPYDATASTRUCT pCopyData = new COPYDATASTRUCT;
		pCopyData->dwData = ShareType_Start;
		pCopyData->lpData = (PVOID)&StartTemp;
		pCopyData->cbData = sizeof(StartTemp);
		::SendMessage(m_WndScreenShare,WM_COPYDATA,WPARAM(m_hWnd), LPARAM(pCopyData));
	}

#endif

	return TRUE;
}

void CVideoDlg::DrawHead(CDC *pDC)
{
	CRect rcTitle;
	CString strTitle;

	if (m_bFullScreen)
		return;

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

void CVideoDlg::InitCtrls()
{
	CRect	rcClient;
	int nScreenWidth = ::GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = ::GetSystemMetrics(SM_CYSCREEN);
	
	GetClientRect(&rcClient);
	
	m_btnMode.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMODE_VIDEO);
	m_btnAudio.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNAUDIO_VIDEO);
	m_btnEndCall.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNENDCALL_VIDEO);
	m_btnScrCap.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNSCRCAP_VIDEO);
	m_btnMore.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNMORE_VIDEO);

	m_cbxRole.Create(WS_VISIBLE | WS_CHILD | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST, CRect(0, 0, 1, 1), this, IDC_CBXROLE_VIDEO);
	m_cbxRole.SetFont(&m_ftDes);

	for (int nIndex = 0; nIndex < 2; nIndex++) {
		CStringA str;

		str.Format("IDS_CHN_ROLE%d", nIndex);
		m_cbxRole.InsertString(nIndex, LANG_STR(str));
	}

	m_btnShow.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNSCR_VIDEO);
	m_btnTip.Create(NULL, WS_VISIBLE | WS_CHILD, CRect(0, 0, 1, 1), this, IDC_BTNTIP_VIDEO);
	

	m_wndLocal.Create(NULL, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, CRect(0, 0, 1, 1), this, IDC_BASEWND_VIDEO + 4);
	m_wndLocal.SetBackImage(IDB_BACKGROUND_VIDEO, 96, 96, RGB(0x44, 0x44, 0x44));
	m_wndLocal.SetFaceColor(RGB(0x58, 0x58, 0x58));
	m_wndLocal.SetUID(0);

	m_btnMin.MoveWindow(rcClient.Width() - 72, 1, 22, 22, TRUE);
	m_btnRst.MoveWindow(rcClient.Width() - 48, 1, 22, 22, TRUE);
	m_btnClose.MoveWindow(rcClient.Width() - 24, 1, 22, 22, TRUE);

	m_btnMode.MoveWindow(rcClient.Width() / 2 - 24, rcClient.Height() - 84, 72, 72, TRUE);
	m_btnAudio.MoveWindow(rcClient.Width() / 2 + 24, rcClient.Height() - 84, 72, 72, TRUE);
	m_btnShow.MoveWindow(rcClient.Width()/2 + 144, rcClient.Height() - 84, 72, 72, TRUE);
	m_btnEndCall.MoveWindow(rcClient.Width() - 120, rcClient.Height() - 84, 72, 72, TRUE);

	m_cbxRole.MoveWindow(rcClient.Width() - 80, rcClient.Height() - 84, 300, 48, TRUE);

	m_btnMin.SetBackImage(IDB_BTNMIN, RGB(0xFF, 0x00, 0xFF));
	m_btnRst.SetBackImage(IDB_BTNMAX, RGB(0xFF, 0x00, 0xFF));
	m_btnClose.SetBackImage(IDB_BTNCLOSE, RGB(0xFF, 0x00, 0xFF));

	m_btnScrCap.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnScrCap.EnableFrameEffect(FALSE);
	m_btnScrCap.SetBackImage(IDB_BTNSCRCAP_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnTip.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnTip.EnableFrameEffect(FALSE);
	m_btnTip.SetBackImage(IDB_BTNTIP_VIDEO, RGB(0x26, 0x26, 0x26));
	m_btnTip.SwitchButtonStatus(CAGButton::AGBTN_PUSH);

	m_btnMore.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnMore.EnableFrameEffect(FALSE);
	m_btnMore.SetBackImage(IDB_BTNMORE_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnMode.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnMode.EnableFrameEffect(FALSE);
	m_btnMode.SetBackImage(IDB_BTNAUDIO_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnAudio.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnAudio.EnableFrameEffect(FALSE);
	m_btnAudio.SetBackImage(IDB_BTNMAUDIO_VIDEO, RGB(0x26, 0x26, 0x26));
	
	m_btnShow.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnShow.EnableFrameEffect(FALSE);
	m_btnShow.SetBackImage(IDB_BTNFULLSCR_VIDEO, RGB(0x26, 0x26, 0x26));

	m_btnEndCall.SetBackColor(RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26), RGB(0x26, 0x26, 0x26));
	m_btnEndCall.EnableFrameEffect(FALSE);
	m_btnEndCall.SetBackImage(IDB_BTNENDCALL_VIDEO, RGB(0x26, 0x26, 0x26));
}

void CVideoDlg::ShowVideo1()
{
	m_wndLocal.ShowWindow(SW_HIDE);
	m_wndLocal.SetBigShowFlag(TRUE);

	m_wndLocal.MoveWindow(&m_rcVideoArea, FALSE);
	m_wndLocal.SetParent(this);
	m_wndLocal.SetBigShowFlag(TRUE);
	m_wndLocal.ShowWindow(SW_SHOW);

// 	if (m_listWndInfo.GetCount() == 0) {
// 		m_wndLocal.MoveWindow(&m_rcVideoArea, FALSE);
// 		m_wndLocal.SetParent(this);
// 		m_wndLocal.SetBigShowFlag(TRUE);
// 		m_wndLocal.ShowWindow(SW_SHOW);
// 	}

	m_nScreenMode = SCREEN_VIDEO1;

	ShowButtonsNormal();
}

void CVideoDlg::ShowMulti()
{
	int nLocalIndex = 0;

	m_wndLocal.ShowWindow(TRUE);
	m_wndLocal.SetBigShowFlag(FALSE);
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		m_wndVideo[nIndex].ShowWindow(SW_HIDE);
		m_wndVideo[nIndex].SetBigShowFlag(FALSE);
	}
	
	if (m_lpBigShowed == NULL)
		m_lpBigShowed = &m_wndVideo[0];

	m_lpBigShowed->ShowWindow(SW_SHOW);
	m_lpBigShowed->MoveWindow(&m_rcVideoArea, TRUE);
	m_lpBigShowed->SetParent(this);
	m_lpBigShowed->SetBigShowFlag(TRUE);
	
	for (int nIndex = 0; nIndex < 4; nIndex++) {
		int nXPos = (m_rcVideoArea.Width() / 2) - 402 + (204 * nLocalIndex);
		int nYPos = m_rcVideoArea.top + 16;

		if (!m_wndVideo[nIndex].IsBigShow()) {
			if (m_wndVideo[nIndex].GetUID() != 0) {
				m_wndVideo[nIndex].MoveWindow(nXPos, nYPos, 192, 144, TRUE);
				m_wndVideo[nIndex].ShowWindow(SW_SHOW);
				m_wndVideo[nIndex].SetParent(m_lpBigShowed);
				nLocalIndex++;
			}
		}
		else{
			m_wndLocal.MoveWindow(nXPos, nYPos, 192, 144, TRUE);
			m_wndLocal.ShowWindow(SW_SHOW);
			m_wndLocal.SetParent(m_lpBigShowed);
			nLocalIndex++;
		}
	}

	m_nScreenMode = SCREEN_VIDEOMULTI;

	ShowButtonsNormal();
}

BOOL CVideoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_dlgDevice.Create(CDeviceDlg::IDD, this);
	m_dlgDevice.EnableDeviceTest(FALSE);

	m_dlgScreenCapture.Create(CAGScreenCaptureDlg::IDD, this);
	m_dlgChat.Create(CChatDlg::IDD, this);
	m_dlgSEI.Create(CSEIDlg::IDD, this);

	m_ftDes.CreateFont(14, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Arial"));

	m_bitMenuDevice.LoadBitmap(IDB_MENU_DEVICE);
	m_bitMenuSEI.LoadBitmap(IDB_MENU_SEI);
	
	InitCtrls();

	m_shareWndDlg.Create(CShareWndDlg::IDD);

	int nNum = 0;
	closeProcess("ProcessScreenShare.exe",nNum);
	DWORD dwProcessId = openProcess("ProcessScreenShare.exe", "");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}


HWND CVideoDlg::GetRemoteVideoWnd(int nIndex)
{
	if (nIndex < 0 || nIndex > 3)
		return NULL;

	return m_wndVideo[nIndex].GetSafeHwnd();
}


void CVideoDlg::RebindVideoWnd(uid_t uid)
{
	if (m_wndVideo[0].GetSafeHwnd() == NULL || m_wndLocal.GetSafeHwnd() == NULL)
		return;

	VideoCanvas canvas;

	canvas.renderMode = RENDER_MODE_FIT;

	POSITION pos = m_listWndInfo.GetHeadPosition();
	if (uid == m_teacherUid)//teacher camera
	{
		VideoCanvas vc;
		IRtcEngine* lpRtcEngine = CAgoraObject::GetAgoraObject()->GetEngine();
		vc.uid = uid;
		vc.view = m_wndLocal.GetSafeHwnd();
		vc.renderMode = RENDER_MODE_TYPE::RENDER_MODE_FIT;
		lpRtcEngine->setupRemoteVideo(vc);
		m_wndLocal.MoveWindow(&m_rcVideoArea, FALSE);
		m_wndLocal.SetParent(this);
		m_wndLocal.SetBigShowFlag(TRUE);
		m_wndLocal.ShowWindow(SW_SHOW);
	}
	else if (uid == m_teacherUid + 1){//teacher screen 
		m_shareWndDlg.MoveWindow(0, 0, 960, 720, 1);
		m_shareWndDlg.ShowWindow(SW_SHOW);

		VideoCanvas canvas;

		canvas.renderMode = RENDER_MODE_FIT;
		canvas.uid = uid;
		canvas.view = m_shareWndDlg.GetTeacherScreenShareWnd();
		IRtcEngine* lpRtcEngine = CAgoraObject::GetAgoraObject()->GetEngine();
		lpRtcEngine->setupRemoteVideo(canvas);
	}
}


BOOL CVideoDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN){
		switch (pMsg->wParam){
		case VK_RETURN:
			return FALSE;
		case VK_ESCAPE:
			if (m_bFullScreen)
				OnBnClickedBtnfullscr();
			
			return FALSE;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CVideoDlg::OnShowModeChanged(WPARAM wParam, LPARAM lParam)
{
// 	if (m_nScreenMode == SCREEN_VIDEOMULTI) {
// 		if (m_listWndInfo.GetCount() <= 1)
// 			ShowVideo1();
// 		else if (m_listWndInfo.GetCount() > 1 && m_listWndInfo.GetCount() < 4)
// 			ShowVideo4();
// 	}
// 	else {
// 		m_lpBigShowed = (CAGVideoWnd *)wParam;
// 		ShowMulti();
// 	}
	
	return 0;
}

LRESULT CVideoDlg::OnShowBig(WPARAM wParam, LPARAM lParam)
{
	CRect rcNextBig;
	CRect rcCurBig;

	if (m_nScreenMode != SCREEN_VIDEOMULTI)
		return 0;

	CAGVideoWnd *lpNextBigWnd = (CAGVideoWnd *)wParam;
	if (lpNextBigWnd == m_lpBigShowed)
		return 0;

	//ShowMulti();
	m_lpBigShowed = lpNextBigWnd;

	return 0;
}


void CVideoDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO:  �ڴ˴������Ϣ����������

	if (bShow && GetSafeHwnd() != NULL) {
		RebindVideoWnd(m_teacherUid);
		int nRole = CAgoraObject::GetAgoraObject()->GetClientRole();

		if (nRole == 1)
			m_cbxRole.SetCurSel(0);
		else
			m_cbxRole.SetCurSel(1);
	}
}