
// MyStockDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyStock.h"
#include "MyStockDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
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

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMyStockDlg 对话框


typedef void (CMyStockDlg::* HotKeyFunc)();

struct HotKeyFuncInfo 
{
	int id;
	UINT fsModifiers;
	UINT vk;
	HotKeyFunc func;
};
// 0X0000-0xBFFF
//#define WM_MYHOTKEY WM_USER+1000 
int nextHotKeyId = 0X0000;
#define HKFITEM(__modifiers, __vk, __func) { ++nextHotKeyId, __modifiers, __vk, __func }

HotKeyFuncInfo g_HotKeyFuncInfos[] = {
	HKFITEM(MOD_CONTROL,VK_UP, &CMyStockDlg::ChartZoomIn)
	,HKFITEM(MOD_CONTROL,VK_DOWN, &CMyStockDlg::ChartZoomOut)
	,HKFITEM(MOD_CONTROL,VK_LEFT, &CMyStockDlg::ChartMoveLeft)
	,HKFITEM(MOD_CONTROL,VK_RIGHT, &CMyStockDlg::ChartMoveRight)
};



CMyStockDlg::CMyStockDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMyStockDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	COLORREF crBG		= RGB(0,0,0);
	COLORREF crUp		= RGB(255,0,0);
	COLORREF crDown		= RGB(0,0,255);
	COLORREF crError	= RGB(255,255,255);
	m_BrushBG.CreateSolidBrush(crBG);
	m_BrushUp.CreateSolidBrush(crUp);
	m_BrushDown.CreateSolidBrush(crDown);
	m_PenUp.CreatePen(PS_SOLID,0,crUp);
	m_PenDown.CreatePen(PS_SOLID,0,crDown);
	m_PenError.CreatePen(PS_SOLID,0,crError);

	m_ChartUnitX = CHARTUNITX_MAX;
	m_ChartUnitXOffest = 0;
}

void CMyStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMyStockDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_KEYUP()
	ON_WM_HOTKEY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON1, &CMyStockDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMyStockDlg 消息处理程序

BOOL CMyStockDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

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
	MoveWindow( 0, 0, 1024, 768 );
	CenterWindow();

	// 0X0000-0xBFFF
	int numHotKey = sizeof(g_HotKeyFuncInfos) / sizeof(HotKeyFuncInfo);
	for ( int i=0; i<numHotKey; ++i )
	{
		HotKeyFuncInfo& hkfi = g_HotKeyFuncInfos[i];
		int nRet = RegisterHotKey( GetSafeHwnd(), hkfi.id, hkfi.fsModifiers, hkfi.vk );
		if (0 == nRet)
		{
			int i=0;
		}
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMyStockDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMyStockDlg::OnDestroy()
{
	int numHotKey = sizeof(g_HotKeyFuncInfos) / sizeof(HotKeyFuncInfo);
	for ( int i=0; i<numHotKey; ++i )
	{
		HotKeyFuncInfo& hkfi = g_HotKeyFuncInfos[i];
		UnregisterHotKey( GetSafeHwnd(), hkfi.id );
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMyStockDlg::OnPaint()
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
		if ( !m_stockTradeDates.empty() )
		{
			CalculateChartView();

			CRect rect;
			GetClientRect(&rect);
			CPaintDC dc(this); // 用于绘制的设备上下文
			

			//定义一个内存设备描述表对象（即后备缓冲区）   
			CDC MemDC;   
			//定义一个位图对象   
			CBitmap MemBitmap;  
			//建立与屏幕设备描述表（前端缓冲区）兼容的内存设备描述表句柄（后备缓冲区）   
			MemDC.CreateCompatibleDC(NULL);  
			//这时还不能绘图，因为没有位图的设备描述表是不能绘图的   
			//下面建立一个与屏幕设备描述表（或者内存设备描述表）兼容的位图   
			MemBitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Width());  
			//将位图选入到内存设备描述表   
			//只有选入了位图的设备描述表才有地方绘图，画到指定的位图上   
			CBitmap *pOldBit=MemDC.SelectObject(&MemBitmap);  
			


			CPen* pOldPen=MemDC.SelectObject(&m_PenUp);
			CBrush* pOldBrush=MemDC.SelectObject(&m_BrushUp);

			DrawChart(MemDC);

			MemDC.SelectObject(pOldPen);
			MemDC.SelectObject(pOldBrush);

			//将后备缓冲区中的图形拷贝到前端缓冲区   
			dc.BitBlt(0,0,rect.Width(),rect.Width(),&MemDC,0,0,SRCCOPY);  
			//绘图完成后的清理   
			MemBitmap.DeleteObject();  
			MemDC.DeleteDC();

		}
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMyStockDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMyStockDlg::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch( nChar )
	{
	case '-':
		break;
	case '=':
		break;

	}
}

void CMyStockDlg::OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2)
{
	int numHotKey = sizeof(g_HotKeyFuncInfos) / sizeof(HotKeyFuncInfo);
	for ( int i=0; i<numHotKey; ++i )
	{
		HotKeyFuncInfo& hkfi = g_HotKeyFuncInfos[i];
		if ( nHotKeyId == hkfi.id )
		{
			(this->*(hkfi.func))();
		}
	} 

}

void CMyStockDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_ptLastMouseLButtonDownPos = point;
}

void CMyStockDlg::OnLButtonUp(UINT nFlags, CPoint point)
{

}

void CMyStockDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_ptLastMousePos == point )
	{
		return;
	}
	m_ptLastMousePos = point;
	
	if ( nFlags & MK_LBUTTON == MK_LBUTTON)
	{

		m_ChartUnitXOffest += (point.x - m_ptLastMouseLButtonDownPos.x);
		if (m_ChartUnitXOffest<0)
		{
			m_ChartUnitXOffest = 0;
		}
		m_ptLastMouseLButtonDownPos = point;
	}

	Invalidate();
	UpdateWindow();

}

BOOL CMyStockDlg::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CMyStockDlg::CalculateChartView()
{
	RECT rt;
	GetClientRect(&rt);
	m_rtChart = rt;
}

void CMyStockDlg::DrawChart(CDC& dc)
{
	DrawBG(dc);

	CRect& rtChart = m_rtChart;
	//int num = 400;
	//int num = 50;
	int num = rtChart.Width()/m_ChartUnitX;
	if ( num > m_stockTradeDates.size() )
	{
		num = m_stockTradeDates.size();
	}
	if ( m_ChartUnitXOffest < 0 )
	{
		m_ChartUnitXOffest = 0;
	}

	if ( (m_ChartUnitXOffest+num) > m_stockTradeDates.size() )
	{
		m_ChartUnitXOffest = m_stockTradeDates.size()-num;
		assert(m_ChartUnitXOffest>=0);
	}
	TRACE("m_ChartUnitXOffest = %d\n", m_ChartUnitXOffest);
	for ( int i=0; i<num; ++i )
	{
		TradeDate& td = m_stockTradeDates[num-i-1+m_ChartUnitXOffest];
		DrawTradeDate( dc, i, num, num-i-1+m_ChartUnitXOffest, td );
	}

	DrawTradeDateDetail(dc, num);
}

void CMyStockDlg::DrawBG(CDC& dc)
{
	CRect& rtChart = m_rtChart;
	dc.FillRect(&rtChart,&m_BrushBG);
}

void CMyStockDlg::DrawTradeDate(CDC& dc, int index, int indexNum, int index2, const TradeDate& td)
{
	if ( td.volume == 0 )
	{
		return;
	}
	DWORD priceLow = 0;
	DWORD priceHigh = 3000;
	float priceDifference = (priceHigh-priceLow);

	CRect& rtChart = m_rtChart;
	int xUnit = rtChart.Width()/indexNum;
	if ( xUnit % 2 == 0 )
	{
		xUnit -= 1;
	}
	else
	{
		xUnit -= 2;
	}
	if ( xUnit < 4 )
	{
		xUnit = 2;
	}
	xUnit = m_ChartUnitX;
	float yUnit = rtChart.Height()/priceDifference;

	if ( td.close >= td.open )
	{
		dc.SelectObject(&m_PenUp);
		dc.SelectObject(&m_BrushUp);
	}
	else
	{
		dc.SelectObject(&m_PenDown);
		dc.SelectObject(&m_BrushDown);
	}
	if ( td.close == td.open )
	{
		dc.SelectObject(&m_PenError);
	}

	POINT high = { rtChart.left+index*xUnit+xUnit/2, rtChart.bottom-yUnit*(td.high-priceLow) };
	POINT low  = { rtChart.left+index*xUnit+xUnit/2, rtChart.bottom-yUnit*(td.low-priceLow) };
	dc.MoveTo(high);
	dc.LineTo(low);

	if ( xUnit > 2 )
	{
		RECT rt = {rtChart.left+index*xUnit,rtChart.bottom-yUnit*(td.close-priceLow),rtChart.left+index*xUnit+xUnit-1,rtChart.bottom-yUnit*(td.open-priceLow)};
		if ( td.close != td.open )
		{

			//dc.FillRect(&rt,new CBrush(RGB(255,255,255)));
			dc.Rectangle(&rt);
		}
		else
		{
			POINT left = { rtChart.left+index*xUnit, rtChart.bottom-yUnit*(td.open-priceLow) };
			POINT right  = { rtChart.left+index*xUnit+xUnit-1, rtChart.bottom-yUnit*(td.close-priceLow) };
			dc.MoveTo(left);
			dc.LineTo(right);
		}

		rt.left = rt.left - 10;
		rt.right = rt.right + 10;
		rt.top	= rtChart.bottom-yUnit*(td.low-priceLow);
		rt.bottom = rt.top + 20;
		char szStr[256] = "";
		sprintf( szStr, "%d", index2 );
		//dc.Rectangle(&rt);
		//dc.SetTextColor(RGB(0,0,0));
		//SetBkColor SetBkMode 		 
		//dc.SetBkMode(TRANSPARENT); //透明背景
		dc.DrawText(szStr, -1,&rt, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		//dc.TextOut(rt.left, rt.top, szStr, -1 );

	}
}

void CMyStockDlg::DrawTradeDateDetail(CDC& dc, int indexNum)
{
	if ( !m_rtChart.PtInRect( m_ptLastMousePos ) )
	{
		return;
	}
	POINT pt = { m_ptLastMousePos.x, m_ptLastMousePos.y };

	CRect& rtChart = m_rtChart;
	int firstIndex = indexNum+m_ChartUnitXOffest;
	int index = (pt.x - m_rtChart.left)/m_ChartUnitX;
	
	if ( firstIndex-index-1 < 0 
		|| firstIndex-index-1 >= m_stockTradeDates.size() )
	{
		return;
	}
	TradeDate& td = m_stockTradeDates[firstIndex-index-1];
	
	if ( td.close >= td.open )
	{
		dc.SelectObject(&m_PenUp);
		dc.SelectObject(&m_BrushUp);
	}
	else
	{
		dc.SelectObject(&m_PenDown);
		dc.SelectObject(&m_BrushDown);
	}

	const int DETAIL_WIDTH = 100;
	const int DETAIL_HEIGHT = 100;
	const int TEXT_HEIGHT = 12;
	RECT rt = { pt.x-DETAIL_WIDTH, pt.y-DETAIL_HEIGHT, pt.x, pt.y };
	dc.Rectangle(&rt);
	dc.SetBkMode(TRANSPARENT); //透明背景
	char szStr[256] = "";
	sprintf( szStr, "[%04d-%02d-%02d]%d", td.year, td.month, td.day, index );
	dc.DrawText(szStr, -1,&rt, DT_SINGLELINE | DT_LEFT );
	sprintf( szStr, " %0.2f", td.open/100.0f );
	rt.top += TEXT_HEIGHT;
	dc.DrawText(szStr, -1,&rt, DT_SINGLELINE | DT_LEFT );
	sprintf( szStr, " %0.2f", td.high/100.0f );
	rt.top += TEXT_HEIGHT;
	dc.DrawText(szStr, -1,&rt, DT_SINGLELINE | DT_LEFT );
	sprintf( szStr, " %0.2f", td.low/100.0f );
	rt.top += TEXT_HEIGHT;
	dc.DrawText(szStr, -1,&rt, DT_SINGLELINE | DT_LEFT );
	sprintf( szStr, " %0.2f", td.close/100.0f );
	rt.top += TEXT_HEIGHT;
	dc.DrawText(szStr, -1,&rt, DT_SINGLELINE | DT_LEFT );

}

void CMyStockDlg::ChartZoomIn()
{
	m_ChartUnitX+=3;
	if( m_ChartUnitX > CHARTUNITX_MAX )
	{
		m_ChartUnitX = CHARTUNITX_MAX;
	}
	if ( m_ChartUnitX % 2 == 0 )
	{
		m_ChartUnitX -= 1;
	}
	else
	{
		m_ChartUnitX -= 2;
	}

	Invalidate();
	UpdateWindow();
}

void CMyStockDlg::ChartZoomOut()
{
	m_ChartUnitX-=3;
	if( m_ChartUnitX < CHARTUNITX_MIN )
	{
		m_ChartUnitX = CHARTUNITX_MIN;
	}
	if ( m_ChartUnitX % 2 == 0 )
	{
		m_ChartUnitX -= 1;
	}
	else
	{
		m_ChartUnitX -= 2;
	}
	Invalidate();
	UpdateWindow();
}

void CMyStockDlg::ChartMoveLeft()
{
	++m_ChartUnitXOffest;

	Invalidate();
	UpdateWindow();
}

void CMyStockDlg::ChartMoveRight()
{
	--m_ChartUnitXOffest;
	if (m_ChartUnitXOffest<0)
	{
		m_ChartUnitXOffest = 0;
	}

	Invalidate();
	UpdateWindow();
}

struct float2DWORD
{
	DWORD h;
	DWORD l;
	DWORD toDWORD(){
		return h*100 + l;
	}
};

void CMyStockDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	// http://table.finance.yahoo.com/table.csv?s=0777.hk
	// http://table.finance.yahoo.com/table.csv?s=0777.hk&f=aa5
	// http://download.finance.yahoo.com/d/quotes.csv?s=0777.hk&f=nd1t1k1k2k3l1v

	const char* fileName = "table.csv";
	FILE* f = fopen( fileName, "r" );
	if ( f )
	{
		char line[256] = "";
		char line2[256] = "";
		char head[] = "Date,Open,High,Low,Close,Volume,Adj Close\n";
		while( fgets( line, sizeof(line), f ) )
		{
			//TRACE( line );
			if ( stricmp(line,head) == 0 )
			{
				continue;
			}
			TradeDate td;
			float2DWORD open;
			float2DWORD high;
			float2DWORD low;
			float2DWORD close;
			float2DWORD adjClose;

			sscanf( line, "%d-%d-%d,%u.%u,%u.%u,%u.%u,%u.%u,%u,%u.%u\n", &td.year, &td.month, &td.day, &open.h, &open.l, &high.h, &high.l, &low.h, &low.l, &close.h, &close.l, &td.volume, &adjClose.h, &adjClose.l );
			td.open = open.toDWORD();
			td.high = high.toDWORD();
			td.low = low.toDWORD();
			td.close = close.toDWORD();
			td.adjClose = adjClose.toDWORD();
			sprintf( line2, "%d-%02d-%02d,%.2f,%.2f,%.2f,%.2f,%03u,%.2f\n", td.year, td.month, td.day, td.open/100.0f, td.high/100.0f, td.low/100.0f, td.close/100.0f, td.volume, td.adjClose/100.0f );
			//TRACE( line2 );
			assert( stricmp(line,line2) == 0 );

			m_stockTradeDates.push_back(td);
		}
		fclose( f );
	}

	Invalidate();
	UpdateWindow();


}
