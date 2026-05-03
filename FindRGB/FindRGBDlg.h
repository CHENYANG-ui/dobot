// FindRGBDlg.h : header file
//

#if !defined(AFX_FINDRGBDLG_H__922B647B_A09C_44D2_A742_F74B18F98832__INCLUDED_)
#define AFX_FINDRGBDLG_H__922B647B_A09C_44D2_A742_F74B18F98832__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

/////////////////////////////////////////////////////////////////////////////
// CFindRGBDlg dialog

class CFindRGBDlg : public CDialog
{
// Construction
public:
	CFindRGBDlg(CWnd* pParent = NULL);	// standard constructor
	IplImage* image;
	IplImage* pic;
	void DrawPicToHDC(IplImage* img,UINT ID);
	CvMemStorage * storage;
    CvSeq * contour;
    int mode;


// Dialog Data
	//{{AFX_DATA(CFindRGBDlg)
	enum { IDD = IDD_FINDRGB_DIALOG };
	CStatic	m_picture;
	CComboBox	m_colorList;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFindRGBDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CFindRGBDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnFind();
	afx_msg void OnClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FINDRGBDLG_H__922B647B_A09C_44D2_A742_F74B18F98832__INCLUDED_)
