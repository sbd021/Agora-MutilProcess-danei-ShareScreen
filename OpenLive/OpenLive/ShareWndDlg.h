#pragma once

#include "AGButton.h"
#include "AGVideoWnd.h"
// CShareWndDlg dialog

class CShareWndDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CShareWndDlg)

public:
	CShareWndDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CShareWndDlg();

// Dialog Data
	enum { IDD = IDD_SHAREWND_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void InitCtrls();

	void DrawHead(CDC *pDC);
	CBrush			m_brHead;
	CAGButton		m_btnMin;
	CAGButton		m_btnRst;
	CAGButton		m_btnClose;

	CAGVideoWnd		m_wndTeacherScreenShare;
	CRect m_rcVideoArea;
public:
	void SetVideoInfo(int fps, int w, int h, int bitrate);
	HWND GetTeacherScreenShareWnd(){ return m_wndTeacherScreenShare.GetSafeHwnd(); }
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedBtnmin();
	afx_msg void OnBnClickedBtnclose();
	afx_msg void OnBnClickedBtnrest();

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnNcHitTest(CPoint point);
};
