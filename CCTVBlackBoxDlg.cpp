
// CCTVBlackBoxDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "CCTVBlackBox.h"
#include "CCTVBlackBoxDlg.h"
#include "afxdialogex.h"
#include "setting.h"
#include "imageProcessing.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CCCTVBlackBoxDlg 대화 상자



CCCTVBlackBoxDlg::CCCTVBlackBoxDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CCTVBLACKBOX_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCCTVBlackBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TIMER, Edit_Timer);
	DDX_Control(pDX, IDC_ALARM, Edit_Alarm);
	DDX_Control(pDX, IDC_SAVE_MIN, Edit_SaveMin);
	DDX_Control(pDX, IDC_SAVE_MAX, Edit_SaveMax);
	DDX_Control(pDX, IDC_THRESHOLD, Edit_Threshold);
	DDX_Control(pDX, IDC_MATCHING, Edit_Matching);
}

BEGIN_MESSAGE_MAP(CCCTVBlackBoxDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CCCTVBlackBoxDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_HIDE, &CCCTVBlackBoxDlg::OnBnClickedHide)
	ON_MESSAGE(WM_TRAY_NOTIFYICACTION, OnTrayNotifyAction)
	ON_BN_CLICKED(IDC_OPEN_FOLDER, &CCCTVBlackBoxDlg::OnBnClickedOpenFolder)
END_MESSAGE_MAP()


// CCCTVBlackBoxDlg 메시지 처리기

BOOL CCCTVBlackBoxDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	SetWindowText(_T("CCTV Black Box"));

	m_font.CreateFont(15,
						0,
						0,
						0,
						FW_NORMAL,
						0,
						0,
						0,
						HANGUL_CHARSET,
						OUT_DEFAULT_PRECIS,
						0,
						DEFAULT_QUALITY,
						DEFAULT_PITCH | FF_DONTCARE,
						_T("굴림체"));

	GetDlgItem(IDC_STATIC)->SetFont(&m_font);

	extern CSetting* setting;
	CString str;
	str.Format(_T("%d"), setting->getTimerInterval());
	Edit_Timer.SetWindowTextW(str);
	str.Format(_T("%d"), setting->getAlarmInterval());
	Edit_Alarm.SetWindowTextW(str);
	str.Format(_T("%d"), setting->getSaveInterval().first);
	Edit_SaveMin.SetWindowTextW(str);
	str.Format(_T("%d"), setting->getSaveInterval().second);
	Edit_SaveMax.SetWindowTextW(str);
	str.Format(_T("%f"), setting->getThreshold());
	Edit_Threshold.SetWindowTextW(str);
	str.Format(_T("%f"), setting->getMatching());
	Edit_Matching.SetWindowTextW(str);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CCCTVBlackBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CCCTVBlackBoxDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CCCTVBlackBoxDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

extern CSetting* setting;

void CCCTVBlackBoxDlg::OnBnClickedOk()
{
	CString str;
	Edit_Timer.GetWindowTextW(str);
	for (int i = 0; i < str.GetLength(); i++) {
		if (str.GetAt(i) < '0' || str.GetAt(i) > '9') {
			MessageBox(_T("타이머 입력 에러"));
			return;
		}
	}
	if(str.GetLength())
		setting->setTimerInterval(_ttoi(str));

	Edit_Alarm.GetWindowTextW(str);
	for (int i = 0; i < str.GetLength(); i++) {
		if (str.GetAt(i) < '0' || str.GetAt(i) > '9') {
			MessageBox(_T("알람 입력 에러"));
			return;
		}
	}
	if (str.GetLength())
		setting->setAlarmInterval(_ttoi(str));

	Edit_SaveMin.GetWindowTextW(str);
	for (int i = 0; i < str.GetLength(); i++) {
		if (str.GetAt(i) < '0' || str.GetAt(i) > '9') {
			MessageBox(_T("저장 주기 입력 에러"));
			return;
		}
	}
	int min = _ttoi(str);

	Edit_SaveMax.GetWindowTextW(str);
	for (int i = 0; i < str.GetLength(); i++) {
		if (str.GetAt(i) < '0' || str.GetAt(i) > '9') {
			MessageBox(_T("저장 주기 입력 에러"));
			return;
		}
	}
	if (str.GetLength())
		setting->setSaveInterval({ min, _ttoi(str) });

	Edit_Threshold.GetWindowTextW(str);
	for (int i = 0; i < str.GetLength(); i++) {
		if ((str.GetAt(i) > '0' && str.GetAt(i) < '9') || str.GetAt(i) == '.') {
			MessageBox(_T("코너 임계값 입력 에러"));
			return;
		}
	}
	if (str.GetLength())
		setting->setThreshold(_ttof(str));

	Edit_Matching.GetWindowTextW(str);
	for (int i = 0; i < str.GetLength(); i++) {
		if ((str.GetAt(i) > '0' && str.GetAt(i) < '9') || str.GetAt(i) == '.') {
			MessageBox(_T("매칭 기준값 입력 에러"));
			return;
		}
	}
	if (str.GetLength())
		setting->setThreshold(_ttof(str));

	MessageBox(_T("업데이트 완료"));

//	CDialogEx::OnOK();
}


void CCCTVBlackBoxDlg::OnBnClickedHide()
{
	NOTIFYICONDATA nid;

	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.uID = 0;
	nid.uFlags = NIF_ICON | NIF_TIP | NIF_MESSAGE;
	nid.hWnd = m_hWnd;
	nid.hIcon = AfxGetApp()->LoadIconW(IDR_MAINFRAME);
	nid.uCallbackMessage = WM_TRAY_NOTIFYICACTION;

	lstrcpy(nid.szTip, _T("CCTV BlackBox"));

	::Shell_NotifyIcon(NIM_ADD, &nid);

	AfxGetApp()->m_pMainWnd->ShowWindow(SW_HIDE);
}

LRESULT CCCTVBlackBoxDlg::OnTrayNotifyAction(WPARAM wParam, LPARAM lParam) {
	switch (lParam) {
		case WM_LBUTTONDBLCLK: {
			NOTIFYICONDATA nid;
			ZeroMemory(&nid, sizeof(nid));
			nid.cbSize = sizeof(nid);
			nid.uID = 0;
			nid.hWnd = GetSafeHwnd();

			BOOL bRet = ::Shell_NotifyIcon(NIM_DELETE, &nid);
			if (!bRet){
				::AfxMessageBox(_T("트레이아이콘 제거실패"));
				return -1;
			}
			AfxGetApp()->m_pMainWnd->ShowWindow(SW_SHOW);
		}break;
	}

	return 0;
}

void CCCTVBlackBoxDlg::OnBnClickedOpenFolder()
{
	ShellExecuteA(NULL, "explore", setting->getImagePath(), NULL, NULL, SW_SHOWNORMAL);
}