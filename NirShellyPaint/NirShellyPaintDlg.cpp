
// NirShellyPaintDlg.cpp : implementation file
//

#include "stdafx.h"
#include "NirShellyPaint.h"
#include "NirShellyPaintDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
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

//////////////////////////////////////////////////////////////////////////
// CNirShellyPaintDlg dialog

CNirShellyPaintDlg::CNirShellyPaintDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CNirShellyPaintDlg::IDD, pParent)
	, isFill(FALSE)
{
	shapeType=RECTANGLE;
	curColor=RGB(0,0,0);
	isPressed=false;
	index=iMax=0;

	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

CNirShellyPaintDlg::~CNirShellyPaintDlg()
{
	int size=myShapes.size();
	for(int i=0;i<size;i++)
		myShapes.erase(myShapes.begin());
}

void CNirShellyPaintDlg::ClearScreen() 
{
	myShapes.clear();
	moves.delAll();
	undo.EnableWindow(0);
	redoPic.EnableWindow(0);
	Invalidate();
}

void CNirShellyPaintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK1, isFill);
	DDX_Control(pDX, IDC_BUTTON1, RecPic);
	DDX_Control(pDX, IDC_BUTTON2, ElipPic);
	DDX_Control(pDX, IDC_BUTTON3, TraPic);
	DDX_Control(pDX, IDC_BUTTON5, LinePic);
	DDX_Control(pDX, IDC_BUTTON6, PenPic);
	DDX_Control(pDX, IDC_BUTTON7, MovePic);
	DDX_Control(pDX, IDC_BUTTON9, ColorPic);
	DDX_Control(pDX, IDC_BUTTON10, redoPic);
	DDX_Control(pDX, IDC_BUTTON11, undo);
}

BEGIN_MESSAGE_MAP(CNirShellyPaintDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	
	ON_BN_CLICKED(IDC_BUTTON1, &CNirShellyPaintDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CNirShellyPaintDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CNirShellyPaintDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CNirShellyPaintDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CNirShellyPaintDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CNirShellyPaintDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_CHECK1, &CNirShellyPaintDlg::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_NEW, &CNirShellyPaintDlg::OnBnClickedNew)
	ON_BN_CLICKED(IDC_BUTTON10, &CNirShellyPaintDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &CNirShellyPaintDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON9, &CNirShellyPaintDlg::OnBnClickedButton9)
\
ON_BN_CLICKED(IDC_BUTTON4, &CNirShellyPaintDlg::OnBnClickedButton4)
ON_BN_CLICKED(IDC_BUTTON8, &CNirShellyPaintDlg::OnBnClickedButton8)
ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////
// CNirShellyPaintDlg message handlers

BOOL CNirShellyPaintDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		//BOOL bNameValid;
		CString strAboutMenu;
		//bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		//ASSERT(bNameValid);
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	RecPic.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP1)));
	ElipPic.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP2)));
	TraPic.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP3)));
	LinePic.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP5)));
	PenPic.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP6)));
	MovePic.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP7)));
	ColorPic.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP9)));
	redoPic.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP10)));
	undo.SetBitmap(LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP11)));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CNirShellyPaintDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNirShellyPaintDlg::OnPaint()
{
	int curIndex = myShapes.size();
	CPaintDC dc (this);
	for(int i=0; i<curIndex; i++)
		myShapes[i]->Draw(&dc);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CNirShellyPaintDlg::OnQueryDragIcon()
{
	//return <HCURSOR>(m_hIcon);
	return static_cast<HCURSOR>(m_hIcon);
}


int ExIndex = -1;
void CNirShellyPaintDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	
	int curIndex = myShapes.size();
	endP=startP=point;
	isPressed=true;
	switch(shapeType)
	{
	case PEN:
		myShapes.push_back(new MyPen(startP.x,startP.y ,curColor));
		moves.move(new MyPen(startP.x,startP.y ,curColor));
		break;
	
	case CHANGE_COLOR:
		{
			for (int i=0; i<curIndex; i++)
			if(myShapes[i]->isEx(point))
			{
				myShapes[i]->SetColor(curColor);
				Invalidate();
			}
			CClientDC dc(this);
			COLORREF c=curColor;
		}
		break;

	case MOVE_OBJECT:
		for (int i=0; i<curIndex; i++)
			if(myShapes[i]->isEx(point))
			{
				ExIndex = i;
			}
		break;
	}
	if (curIndex < myShapes.size())
		redoPic.EnableWindow(0);

	CDialog::OnLButtonDown(nFlags, point);
}

void CNirShellyPaintDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if(isPressed)
	{
		int curIndex = myShapes.size();
		CClientDC dc(this);
		CBrush myBrush,*oldBrush;
		if (isFill)
		{
			myBrush.CreateSolidBrush(curColor);		// fill figure color
			oldBrush=dc.SelectObject( &myBrush );
		}
		dc.SetROP2(R2_NOTXORPEN);

		CPen myPen1(PS_SOLID,  4, curColor);		// outside line figure color
		CPen *oldPen;
		oldPen=dc.SelectObject( &myPen1 ); 

		switch(shapeType)
		{
		case RECTANGLE:
			dc.Rectangle(startP.x, startP.y,endP.x, endP.y);
			endP=point;
			dc.Rectangle(startP.x, startP.y,endP.x, endP.y);
			undo.EnableWindow(1);
			break;
		case ELLIPSE:
			dc.Ellipse(startP.x, startP.y,endP.x, endP.y);
			endP=point;
			dc.Ellipse(startP.x, startP.y,endP.x, endP.y);
			undo.EnableWindow(1);
			break;
		case TRIANGLE:
			dc.MoveTo(startP.x,endP.y);
			dc.LineTo(endP.x,endP.y);
			dc.LineTo((startP.x + endP.x)/2,startP.y);
			dc.LineTo(startP.x,endP.y);
			endP=point;
			dc.MoveTo(startP.x,endP.y);
			dc.LineTo(endP.x,endP.y);
			dc.LineTo((startP.x + endP.x)/2,startP.y);
			dc.LineTo(startP.x,endP.y);
			undo.EnableWindow(1);
			break;
		case LINE:
			dc.MoveTo(startP.x,startP.y);
			dc.LineTo(endP.x,endP.y);
			endP=point;
			dc.MoveTo(startP.x,startP.y);
			dc.LineTo(endP.x,endP.y);
			undo.EnableWindow(1);
			break;
		case PEN:
			myShapes[curIndex-1]->Build(point);
			dc.MoveTo(myShapes[curIndex-1]->lastP());
			dc.LineTo(endP.x,endP.y);
			endP=point;
			dc.MoveTo(myShapes[curIndex-1]->lastP());
			dc.LineTo(endP.x,endP.y);
			undo.EnableWindow(1);
			break;

		case MOVE_OBJECT:
			if (ExIndex != -1)
			{
				
				
					myShapes[ExIndex]->Draw(&dc);

			     //myShapes[ExIndex]->x2 += endP.x - myShapes[ExIndex]->x1;


					//myShapes[ExIndex]->y2 += endP.y - myShapes[ExIndex]->y1;

					myShapes[ExIndex]->x1 = endP.x;
					myShapes[ExIndex]->y1 = endP.y;
					endP = point;
					myShapes[ExIndex]->Draw(&dc);

					
			



			}
			break;
		}

	}
	
	CDialog::OnMouseMove(nFlags, point);
}

void CNirShellyPaintDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int curIndex = myShapes.size();
	if(isPressed)
	{
		Invalidate();
		endP=point;
		isPressed=false;
		if (endP != startP)
		{
			switch(shapeType)
			{
			case RECTANGLE:
				myShapes.push_back(new MyRectangle(startP.x,startP.y,endP.x,endP.y,isFill ,curColor));
				moves.move(new MyRectangle(startP.x,startP.y,endP.x,endP.y,isFill ,curColor));
				Invalidate();
				break;
			case ELLIPSE:
				myShapes.push_back(new MyEllipse(startP.x,startP.y,endP.x,endP.y,isFill ,curColor));
				moves.move(new MyEllipse(startP.x,startP.y,endP.x,endP.y,isFill ,curColor));
				Invalidate();
				break;
			case TRIANGLE:
				myShapes.push_back(new MyTriangle(startP.x,startP.y,endP.x,endP.y,isFill ,curColor));
				moves.move(new MyTriangle(startP.x,startP.y,endP.x,endP.y,isFill ,curColor));
				Invalidate();
				break;
			case LINE:
				myShapes.push_back(new MyLine(startP.x,startP.y,endP.x,endP.y ,curColor));
				moves.move(new MyLine(startP.x,startP.y,endP.x,endP.y ,curColor));
				Invalidate();
				break;
			case PEN:
				Invalidate();
				break;
			case MOVE_OBJECT:
				Invalidate();
				if (ExIndex != -1)
				{
					ExIndex = -1;
					Invalidate();
				}
				break;
			}
		}
		if (curIndex < myShapes.size())
			redoPic.EnableWindow(0);
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CNirShellyPaintDlg::OnBnClickedButton1()
{
	shapeType=RECTANGLE;
}

void CNirShellyPaintDlg::OnBnClickedButton2()
{
	shapeType=ELLIPSE;
}

void CNirShellyPaintDlg::OnBnClickedButton3()
{
	shapeType=TRIANGLE;
}


void CNirShellyPaintDlg::OnBnClickedButton5()
{
	shapeType=LINE;
}

void CNirShellyPaintDlg::OnBnClickedButton6()
{
	shapeType=PEN;
}

void CNirShellyPaintDlg::OnBnClickedButton7()
{

	shapeType=MOVE_OBJECT;
}


void CNirShellyPaintDlg::OnBnClickedCheck1() //fill
{
	UpdateData(true); 
}


void CNirShellyPaintDlg::OnBnClickedNew()
{
	if(AfxMessageBox(102, MB_YESNO | MB_ICONQUESTION)==IDYES)
		this->ClearScreen();
}

void CNirShellyPaintDlg::OnBnClickedButton10() //redo
{
	myShapes = moves.Redo();
	undo.EnableWindow(1);
	Invalidate();
}

void CNirShellyPaintDlg::OnBnClickedButton11() //undo
{
	myShapes = moves.Undo();
	if(myShapes.empty())
		undo.EnableWindow(0);
	redoPic.EnableWindow(1);
	Invalidate();
}


void CNirShellyPaintDlg::OnBnClickedButton9() //color
{
	CColorDialog dlg; 
	if (dlg.DoModal() == IDOK) 
		curColor = dlg.GetColor();
}



void CNirShellyPaintDlg::OnBnClickedButton4() // Save button
{
	if (AfxMessageBox(165, MB_YESNO | MB_ICONQUESTION) == IDYES)
	{
		CFile file(_T("File.$$"), CFile::modeWrite);
		CArchive archive(&file, CArchive::store);
		int size = myShapes.size();
		archive << size;
		for (int i = 0;i<size;i++)
		{
			archive << myShapes[i]->style;
			archive << myShapes[i]->isFill;
			archive << myShapes[i]->bgColor;
			archive << myShapes[i]->x1;
			archive << myShapes[i]->y1;
			archive << myShapes[i]->x2;
			archive << myShapes[i]->y2;
		}
		archive.Close();
		file.Close();
	}
}


void CNirShellyPaintDlg::OnBnClickedButton8()//  Load
{
	CFile file(_T("File.$$"), CFile::modeRead);
	CArchive archive(&file, CArchive::load);
	myShapes.clear();
	index = iMax = 0;
	int size;
	CPen p;
	COLORREF color;
	archive >> size;
	int x1, x2, y1, y2, Fill, Thin, style;
	for (int i = 0; i < size; i++)
	{
		archive >> style;
		archive >> Fill;
		archive >> color;
		archive >> x1;
		archive >> y1;
		archive >> x2;
		archive >> y2;
		switch (style)
		{
		case 0:
			myShapes.push_back(new MyRectangle(x1, y1, x2, y2, Fill, color));
			break;
		case 1:
			myShapes.push_back(new MyEllipse(x1, y1, x2, y2, Fill,  color));
			break;
		case 2:
			myShapes.push_back(new MyTriangle(x1, y1, x2, y2, Fill, color));
			break;
		case 3:
			myShapes.push_back(new MyLine(x1, y1, x2, y2, color));
			break;
		case 4:
			myShapes.push_back(new MyPen(x1, y1,color));
			break;
			index++;
		}
	}
	//// TODO: Add your control notification handler code here
	archive.Close();
	file.Close();
	Invalidate();
}


