
// MyStockDlg.h : 头文件
//

#pragma once

#include <vector>

struct TradeDate
{
	int year;
	int month;
	int day;
	DWORD open;
	DWORD high;
	DWORD low;
	DWORD close;
	DWORD volume;
	DWORD adjClose;
};

// CMyStockDlg 对话框
class CMyStockDlg : public CDialogEx
{
// 构造
public:
	CMyStockDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MYSTOCK_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	void ChartZoomIn();
	void ChartZoomOut();
	void ChartMoveLeft();
	void ChartMoveRight();

// 实现
protected:
	enum{
		CHARTUNITX_MAX = 20
		,CHARTUNITX_MIN = 2
	};

	HICON m_hIcon;
	typedef std::vector<TradeDate> TradeDates;
	TradeDates m_stockTradeDates;

	CRect	m_rtChart;
	CBrush	m_BrushBG;
	CBrush	m_BrushUp;
	CBrush	m_BrushDown;
	CPen	m_PenUp;
	CPen	m_PenDown;
	CPen	m_PenError;

	int		m_ChartUnitX;
	int		m_ChartUnitXOffest;

	CPoint	m_ptLastMousePos;
	CPoint	m_ptLastMouseLButtonDownPos;

	void CalculateChartView();
	void DrawChart(CDC& dc);
	void DrawBG(CDC& dc);
	void DrawTradeDate(CDC& dc, int index, int indexNum, int index2, const TradeDate& td);
	void DrawTradeDateDetail(CDC& dc, int indexNum);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnHotKey(UINT nHotKeyId, UINT nKey1, UINT nKey2);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
};
