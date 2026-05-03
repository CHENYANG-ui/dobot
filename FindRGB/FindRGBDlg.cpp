// FindRGBDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FindRGB.h"
#include "FindRGBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();
	
	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
	// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindRGBDlg dialog

CFindRGBDlg::CFindRGBDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFindRGBDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFindRGBDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFindRGBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFindRGBDlg)
	DDX_Control(pDX, IDC_PICTURE, m_picture);
	DDX_Control(pDX, IDC_COLORLIST, m_colorList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFindRGBDlg, CDialog)
//{{AFX_MSG_MAP(CFindRGBDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_BN_CLICKED(IDOK, OnFind)
	ON_BN_CLICKED(IDC_CLEAR, OnClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFindRGBDlg message handlers

BOOL CFindRGBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Add "About..." menu item to system menu.
	
	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
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
	image=cvLoadImage("3.jpg",-1);
	m_colorList.SetCurSel(2);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFindRGBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
//draw picture
void CFindRGBDlg::DrawPicToHDC(IplImage* img,UINT ID)
{
	CDC *pDC = GetDlgItem(ID)->GetDC(); 
	HDC hDC= pDC->GetSafeHdc(); 
	CRect rect; 
	GetDlgItem(ID)->GetClientRect(&rect); 
	CvvImage cimg; 
	
	cimg.CopyOf(img); 
	cimg.DrawToHDC(hDC,&rect); 
	//cvWaitKey(20);
	ReleaseDC(pDC);
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFindRGBDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		if(image!=NULL)
		{
			//image=cvLoadImage("1.jpg",-1);
			DrawPicToHDC(image,IDC_PICTURE);
			//CDialog::OnPaint();
		}
		else
		{
			CDialog::OnPaint();
		}
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFindRGBDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CFindRGBDlg::OnFind() 
{
	int color=m_colorList.GetCurSel();
	
	pic=cvCreateImage( cvSize(image->width,image->height), 8, 1 );
	cvZero(pic);
	for(int x=0;x<image->height;x++)
	{
		for(int y=0;y<image->width;y++) 
		{
			uchar* ptrImg = &CV_IMAGE_ELEM(image,uchar,x,y*3);
		//	uchar* ptrPic = &((uchar*)(pic->imageData + pic->widthStep*y))[x];
			//red
			if(color==0)
			{
				if((ptrImg[0]-ptrImg[1])>200&&(ptrImg[0]-ptrImg[2])>200)
					CV_IMAGE_ELEM(pic,uchar,x,y)=255;
			}
			//Green
			else if(color==1)
			{
				if((ptrImg[1]-ptrImg[0])>200&&(ptrImg[1]-ptrImg[2])>200)
					CV_IMAGE_ELEM(pic,uchar,x,y)=255;
			}
			//blue
			else if(color==2)
			{
				if((ptrImg[2]-ptrImg[0])>200&&(ptrImg[2]-ptrImg[1])>200)
					CV_IMAGE_ELEM(pic,uchar,x,y)=255;
			}
			
		}
	}

	cvNamedWindow("temp",-1);
	cvShowImage("temp",pic);
	cvWaitKey();


	storage = cvCreateMemStorage(0);
	contour = 0;
	mode = CV_RETR_EXTERNAL;
	cvFindContours( pic, storage, &contour, sizeof(CvContour), 
		  mode, CV_CHAIN_APPROX_SIMPLE);
	cvDrawContours(image, contour, 
		 CV_RGB(0,0,0), CV_RGB(0, 0, 0), 
		 2, 2, 8);

	CRect rect; 
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rect); 
	InvalidateRect(rect,true);	
}

void CFindRGBDlg::OnClear() 
{
	// TODO: Add your control notification handler code here
	image=cvLoadImage("3.jpg",-1);
	CRect rect; 
	GetDlgItem(IDC_PICTURE)->GetClientRect(&rect); 
	InvalidateRect(rect,true);
	
}
