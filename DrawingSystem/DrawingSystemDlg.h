
// DrawingSystemDlg.h: 头文件
//

#pragma once

enum DrawingMode
{
	PointMode,
	LineMode,
	EllipseMode,
	RectangleMode,
	OwnerDrawingMode,
	TextMode,
	ImageMode
};

// CDrawingSystemDlg 对话框
class CDrawingSystemDlg : public CDialogEx
{
// 构造
public:
	CDrawingSystemDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DRAWINGSYSTEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	COLORREF LineColor;
	COLORREF ShapeColor;
	
	bool LineType;	//false:实线	true：虚线
	bool isfilled;	//false:无填充	true:有填充
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	int LineWidth;
	afx_msg void OnEnChangeEdit1();
	int m_LineType;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	CComboBox m_fill;
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	CPoint DownPoint;	//鼠标左键按下时候的点
	CPoint UpPoint;	//鼠标左键松开时候的点
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	int Mode;
	void DrawLine();
	void DrawEllipse();
	CComboBox m_Mode;
	afx_msg void OnCbnSelchangeCombo2();

	void DrawRectangle();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	bool startdrawing;
	CPoint lastpoint;
	CString m_text;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
