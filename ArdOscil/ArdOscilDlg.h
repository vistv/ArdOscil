// ArdOscilDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include <CSLScope.h>
#include "SerialClass.h"
#include "afxcoll.h"
#include "atltime.h"
#include "atlbase.h"

#define ID_TIMER_1 100

#define WM_MYMESSAGE WM_USER+1

// CArdOscilDlg dialog
class CArdOscilDlg : public CDialog
{
// Construction
public:
	CArdOscilDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_ARDOSCIL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	CTSLScope Scope;
	int Counter;

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	HACCEL ha;
	CStatic m_Scope;
	afx_msg void OnBnClickedPlotButton();
	afx_msg void OnBnClickedClearButton();
	//CString m_result;
	CString m_number;
//	afx_msg void OnBnClickedStartButton1();
	Serial* SP;
//	afx_msg void OnBnClickedStopButton2();
private:
	char strbuff[1024];
	static UINT ThreadFn(LPVOID pParam);
public:
	
	afx_msg void OnBnClickedButton1();
	void DoIt(void);
	
	


	LRESULT Mess(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnBnClickedButton2();
	bool m_StopThread;
	bool m_ThreadStarted;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int readAll;
	

//	int m_debuggg;
	float MainYArray[1024];
	float MainXArray[1024];
	int currentZoom;
	
	afx_msg void OnBnClickedSave();
	CString paramStr;
	afx_msg void OnBnClickedClear();
	CString FileName;
	CString cinaKanala;
	CComboBox combo;
	CString combostr;
	afx_msg void OnCbnSelchangeCombo1();
	int countmax;
	int readmax;
	CTime startTime;
	CTime stopTime;
	long timeOfMeas;
	int m_begin;
	int m_end;
	float calcTime;
	afx_msg void OnBnClickedCalc();
	afx_msg void OnBnClickedClr();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedExit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CString comport;
	afx_msg void OnSelchangeCombo2();
	CComboBox comport_ctrl;
	CRegKey key;
};
