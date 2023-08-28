
// DrawingSystemDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DrawingSystem.h"
#include "DrawingSystemDlg.h"
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
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CDrawingSystemDlg 对话框



CDrawingSystemDlg::CDrawingSystemDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DRAWINGSYSTEM_DIALOG, pParent)
	, LineWidth(0)
	, m_LineType(0)
	, m_text(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDrawingSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, LineWidth);
	DDV_MinMaxInt(pDX, LineWidth, 0, 20);
	DDX_Radio(pDX, IDC_RADIO1, m_LineType);
	DDX_Control(pDX, IDC_COMBO1, m_fill);
	DDX_Control(pDX, IDC_COMBO2, m_Mode);
	DDX_Text(pDX, IDC_EDIT2, m_text);
}

BEGIN_MESSAGE_MAP(CDrawingSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CDrawingSystemDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDrawingSystemDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CDrawingSystemDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_RADIO1, &CDrawingSystemDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CDrawingSystemDlg::OnBnClickedRadio2)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CDrawingSystemDlg::OnCbnSelchangeCombo1)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_CBN_SELCHANGE(IDC_COMBO2, &CDrawingSystemDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDrawingSystemDlg::OnBnClickedButton3)
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_BUTTON4, &CDrawingSystemDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDrawingSystemDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDrawingSystemDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CDrawingSystemDlg 消息处理程序

BOOL CDrawingSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	LineColor = RGB(0, 0, 255);
	ShapeColor = RGB(255, 0, 0);

	LineWidth = 5;

	m_fill.AddString(TEXT("有填充"));
	m_fill.AddString(TEXT("无填充"));
	m_fill.SetCurSel(1);

	m_Mode.AddString(TEXT("画点"));
	m_Mode.AddString(TEXT("画线"));
	m_Mode.AddString(TEXT("画椭圆"));
	m_Mode.AddString(TEXT("画矩形"));
	m_Mode.AddString(TEXT("自由绘制"));
	m_Mode.SetCurSel(1);

	UpdateData(FALSE);

	Mode = LineMode;

	startdrawing = false;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDrawingSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDrawingSystemDlg::OnPaint()
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
		//CPaintDC dc(this); // 用于绘制的设备上下文
		//dc.SetPixel(50, 50, RGB(255, 0, 0));
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDrawingSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDrawingSystemDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CColorDialog colorDlg(LineColor);         // 构造对话框，传入初始颜色值   
	if (IDOK == colorDlg.DoModal()) // 显示对话框并判断是否点击“确定”   
	{
		LineColor = colorDlg.GetColor();    // 获取颜色对话框中选择的颜色值   

	}

}


void CDrawingSystemDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CColorDialog colorDlg(ShapeColor);         // 构造对话框，传入初始颜色值   
	if (IDOK == colorDlg.DoModal()) // 显示对话框并判断是否点击“确定”   
	{
		ShapeColor = colorDlg.GetColor();    // 获取颜色对话框中选择的颜色值   

	}
}


void CDrawingSystemDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	//CString str;
	//str.Format(TEXT("%d"), LineWidth);
	//MessageBox(str);
}


void CDrawingSystemDlg::OnBnClickedRadio1()
{
	// TODO: 在此添加控件通知处理程序代码
	LineType = false;
}


void CDrawingSystemDlg::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	LineType = true;
}


void CDrawingSystemDlg::OnCbnSelchangeCombo1()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_fill.GetCurSel();

	if(index == 0) isfilled = true;
	else isfilled = false;

	//CString str;
	//str.Format(TEXT("%d"), index);
	//MessageBox(str);
}


void CDrawingSystemDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	

	DownPoint = point;

	if (Mode == PointMode)
	{
		CClientDC dc(this); // 用于绘制的设备上下文
		dc.SetPixel(point.x, point.y, LineColor);
	}
	else if (Mode == OwnerDrawingMode)
	{
		startdrawing = true;
		lastpoint = point;
	}
	else if (Mode == TextMode)
	{
		CClientDC dc(this); // 用于绘制的设备上下文
		dc.TextOut(point.x, point.y, m_text);
		OnCbnSelchangeCombo2();	//切换回原来的模式
	}
	
	OnCbnSelchangeCombo2();	//切换回原来的模式

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDrawingSystemDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	UpPoint = point;

	switch (Mode)
	{
	case LineMode:
		DrawLine();
		break;
	case EllipseMode:
		DrawEllipse();
		break;
	case RectangleMode:
		DrawRectangle();
		break;
	case OwnerDrawingMode:
		startdrawing = false;
		break;
	default:
		;
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}

void CDrawingSystemDlg::DrawLine()
{
	CClientDC dc(this); // 用于绘制的设备上下文
	
	// 创建红色的画笔
	CPen pen(LineType, LineWidth, LineColor);

	// 选择画笔到设备上下文
	CPen* pOldPen = dc.SelectObject(&pen);

	// 绘制直线
	dc.MoveTo(DownPoint.x, DownPoint.y); // 起始点坐标
	dc.LineTo(UpPoint.x, UpPoint.y); // 终点坐标

	// 恢复原始的画笔
	dc.SelectObject(pOldPen);
}

void CDrawingSystemDlg::DrawEllipse()
{
	CClientDC dc(this); // 用于绘制的设备上下文

	// 创建红色的轮廓画笔
	CPen outlinePen(LineType, LineWidth, LineColor);

	// 选择轮廓画笔到设备上下文
	CPen* pOldPen = dc.SelectObject(&outlinePen);
	CBrush* pOldBrush;

	// 创建蓝色的填充画刷
	CBrush fillBrush(ShapeColor);
	// 创建透明的画刷
	CBrush transparentBrush;
	transparentBrush.CreateStockObject(NULL_BRUSH);

	if (isfilled)
	{
		// 选择填充画刷到设备上下文
		pOldBrush = dc.SelectObject(&fillBrush);
	}
	else
	{
		// 选择透明画刷到设备上下文
		pOldBrush = dc.SelectObject(&transparentBrush);
	}


	// 绘制椭圆，左上角和右下角的矩形坐标确定椭圆的边界
	dc.Ellipse(DownPoint.x, DownPoint.y, UpPoint.x, UpPoint.y);

	// 恢复原始的画笔和画刷
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}


void CDrawingSystemDlg::OnCbnSelchangeCombo2()
{
	// TODO: 在此添加控件通知处理程序代码
	int index = m_Mode.GetCurSel();

	if (index == 0) Mode = PointMode;
	else if(index == 1) Mode = LineMode;
	else if (index == 2) Mode = EllipseMode;
	else if (index == 3) Mode = RectangleMode;
	else if (index == 4) Mode = OwnerDrawingMode;
	else Mode = LineMode;
}

void CDrawingSystemDlg::DrawRectangle()
{
	CClientDC dc(this); // 用于绘制的设备上下文

	// 创建红色的轮廓画笔
	CPen outlinePen(LineType, LineWidth, LineColor);

	// 选择轮廓画笔到设备上下文
	CPen* pOldPen = dc.SelectObject(&outlinePen);
	CBrush* pOldBrush;

	// 创建蓝色的填充画刷
	CBrush fillBrush(ShapeColor);
	// 创建透明的画刷
	CBrush transparentBrush;
	transparentBrush.CreateStockObject(NULL_BRUSH);

	if (isfilled)
	{
		// 选择填充画刷到设备上下文
		pOldBrush = dc.SelectObject(&fillBrush);
	}
	else
	{
		// 选择透明画刷到设备上下文
		pOldBrush = dc.SelectObject(&transparentBrush);
	}


	// 绘制矩形
	dc.Rectangle(DownPoint.x, DownPoint.y, UpPoint.x, UpPoint.y);

	// 恢复原始的画笔和画刷
	dc.SelectObject(pOldPen);
	dc.SelectObject(pOldBrush);
}



void CDrawingSystemDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码

	// 调用Invalidate函数来清空绘图内容并触发重绘
	Invalidate();
}



void CDrawingSystemDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (Mode == OwnerDrawingMode && startdrawing)
	{
		//point：当前时刻的点（直线的终点）
		//lastpoint：上一时刻的点（直线的起点）

		CClientDC dc(this); // 用于绘制的设备上下文

		// 创建红色的画笔
		CPen pen(LineType, LineWidth, LineColor);

		// 选择画笔到设备上下文
		CPen* pOldPen = dc.SelectObject(&pen);

		// 绘制直线
		dc.MoveTo(lastpoint.x, lastpoint.y); // 起始点坐标
		dc.LineTo(point.x, point.y); // 终点坐标

		// 恢复原始的画笔
		dc.SelectObject(pOldPen);

		lastpoint = point;	//画完以后，当前时刻的起点就会变为下一时刻的终点
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDrawingSystemDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	Mode = TextMode;
	UpdateData(TRUE);
	MessageBox(TEXT("请左键点击对话框选择插入文字的位置"), TEXT("提示"));

}


void CDrawingSystemDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	
	Mode = ImageMode;

	// TODO: Add your control notification handler code here   
	// 设置过滤器   
	TCHAR szFilter[] = _T("图片(*.png)|*.png|所有文件(*.*)|*.*||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("png"), NULL, 0, szFilter, this);
	CString strFilePath;

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strFilePath = fileDlg.GetPathName();

		CClientDC dc(this); // 用于绘制的设备上下文
		CRect rect;
		GetClientRect(&rect);
		CImage image;
		image.Load(strFilePath);
		image.BitBlt(dc, 0, 0, rect.Width(), rect.Height(), 0, 0);
		image.Destroy();    //没有Destroy()会有内存泄漏。Detach();不行的。
	}

}


void CDrawingSystemDlg::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	CClientDC dc(this);
	CRect rect;
	BOOL  showMsgTag;                  //是否要弹出”图像保存成功对话框" 
	GetClientRect(&rect);                  //获取画布大小
	HBITMAP hbitmap = CreateCompatibleBitmap(dc, rect.right - rect.left, rect.bottom - rect.top);

	//创建兼容位图
	HDC hdc = CreateCompatibleDC(dc);      //创建兼容DC，以便将图像保存为不同的格式
	HBITMAP hOldMap = (HBITMAP)SelectObject(hdc, hbitmap);
	//将位图选入DC，并保存返回值 
	BitBlt(hdc, 0, 0, rect.right - rect.left, rect.bottom - rect.top, dc, 0, 0, SRCCOPY);
	//将屏幕DC的图像复制到内存DC中
	CImage image;
	image.Attach(hbitmap);                //将位图转化为一般图像

	showMsgTag = TRUE;
	CString  strFilter = _T("位图文件(*.bmp)|*.bmp|JPEG 图像文件|*.jpg|GIF图像文件 | *.gif | PNG图像文件 | *.png |其他格式(*.*) | *.* || ");
	CFileDialog dlg(FALSE, _T("bmp"), _T("iPaint1.bmp"), NULL, strFilter);
	if (dlg.DoModal() != IDOK)
			return;

	CString strFileName;          //如果用户没有指定文件扩展名，则为其添加一个
	CString strExtension;
	strFileName = dlg.m_ofn.lpstrFile;
	if (dlg.m_ofn.nFileExtension == 0)               //扩展名项目为0
	{
		switch (dlg.m_ofn.nFilterIndex)
		{
		case 1:
			strExtension = "bmp"; break;
		case 2:
			strExtension = "jpg"; break;
		case 3:
			strExtension = "gif"; break;
		case 4:
			strExtension = "png"; break;
		default:
			break;
		}
		strFileName = strFileName + "." + strExtension;
	}

	CString	saveFilePath = strFileName;     //saveFilePath为视类中的全局变量,类型为CString

	HRESULT hResult = image.Save(saveFilePath);     //保存图像
	if (FAILED(hResult))
	{
		MessageBox(_T("保存图像文件失败！"));
	}
	else
	{
		if (showMsgTag)
			MessageBox(_T("文件保存成功！"));

	}
	image.Detach();
	SelectObject(hdc, hOldMap);
}
