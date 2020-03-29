
// CCTVBlackBoxDlg.h: 헤더 파일
//

#pragma once

#define WM_TRAY_NOTIFYICACTION 0x8031
#define WM_TRAY_CHANGE 0x8032

// CCCTVBlackBoxDlg 대화 상자
class CCCTVBlackBoxDlg : public CDialogEx
{
// 생성입니다.
public:
	CCCTVBlackBoxDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCTVBLACKBOX_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	CFont m_font;
	CButton m_debug;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEdit Edit_Timer;
	CEdit Edit_Alarm;
	CEdit Edit_SaveMin, Edit_SaveMax;
	CEdit Edit_Threshold;
	CEdit Edit_Matching;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedHide();
	LRESULT OnTrayNotifyAction(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedOpenFolder();
	afx_msg void OnBnClickedDebug();
	afx_msg void OnBnClickedCheck1();
	CButton m_alarm;
	afx_msg void OnBnClickedAlarmonoff();
};