// ArdOscilDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ArdOscil.h"
#include "ArdOscilDlg.h"
#include <ctime>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

const int WM_USERMSG=WM_USER+100; //повідомлення від потоку
//http://www.firststeps.ru/mfc/steps/r.php?340


// CArdOscilDlg dialog




CArdOscilDlg::CArdOscilDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArdOscilDlg::IDD, pParent)
	, readAll(0)
	, currentZoom(0)
	, paramStr(_T(""))
	, FileName(_T(""))
	, cinaKanala("6.23")
	, combostr(_T(""))
	, countmax(0)
	, readmax(0)
	, startTime(0)
	, stopTime(0)
	, timeOfMeas(0)
	, m_begin(0)
	, m_end(0)
	, calcTime(0)
	, comport(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	

}

void CArdOscilDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCOPE, m_Scope);
	//DDX_Text(pDX, IDC_RESULT, m_result);
	DDX_Text(pDX, IDC_NUMBER, m_number);
	//  DDX_Text(pDX, IDC_NUMBER2, m_debuggg);

	DDX_Text(pDX, IDC_EDIT1, FileName);
	DDX_Text(pDX, IDC_DIV, cinaKanala);
	DDX_Control(pDX, IDC_COMBO1, combo);
	DDX_CBString(pDX, IDC_COMBO1, combostr);
	DDX_Text(pDX, IDC_BEGIN, m_begin);
	DDV_MinMaxInt(pDX, m_begin, 0, 1023);
	DDX_Text(pDX, IDC_END, m_end);
	DDV_MinMaxInt(pDX, m_end, 0, 1023);
	DDX_Text(pDX, IDC_CT, calcTime);
	DDX_CBString(pDX, IDC_COMBO2, comport);
	DDX_Control(pDX, IDC_COMBO2, comport_ctrl);
}

BEGIN_MESSAGE_MAP(CArdOscilDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PLOT_BUTTON, &CArdOscilDlg::OnBnClickedPlotButton)
	ON_BN_CLICKED(IDC_CLEAR_BUTTON, &CArdOscilDlg::OnBnClickedClearButton)
//	ON_BN_CLICKED(IDC_START_BUTTON1, &CArdOscilDlg::OnBnClickedStartButton1)
//	ON_BN_CLICKED(IDC_STOP_BUTTON2, &CArdOscilDlg::OnBnClickedStopButton2)
	ON_BN_CLICKED(IDC_BUTTON1, &CArdOscilDlg::OnBnClickedButton1)
 	ON_MESSAGE(WM_USERMSG,Mess) //повідомлення від потоку
	
	ON_BN_CLICKED(IDC_BUTTON2, &CArdOscilDlg::OnBnClickedButton2)
	ON_WM_TIMER()
	
	ON_BN_CLICKED(IDC_SAVE, &CArdOscilDlg::OnBnClickedSave)
	ON_BN_CLICKED(IDC_CLEAR, &CArdOscilDlg::OnBnClickedClear)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CArdOscilDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_CALC, &CArdOscilDlg::OnBnClickedCalc)
	ON_BN_CLICKED(IDC_CLR, &CArdOscilDlg::OnBnClickedClr)
	ON_BN_CLICKED(IDOK, &CArdOscilDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CArdOscilDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDEXIT, &CArdOscilDlg::OnBnClickedExit)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CArdOscilDlg::OnSelchangeCombo2)
END_MESSAGE_MAP()


// CArdOscilDlg message handlers



BOOL CArdOscilDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	ha = LoadAccelerators( AfxGetResourceHandle(),MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	

	//SP = new Serial("COM3");

	m_ThreadStarted=false;
	m_StopThread=false;




//	VCL_InitControls( m_hWnd );
	Scope.Open( m_Scope.m_hWnd );
	//Scope.Channels.Add();



	CString tstr = "Decay Curve";
	Scope.Channels[ 0 ].Name = "Decay Curve";
//	Scope.Title="sdg";
	
	
	readmax=0;
	countmax=64000;
	cinaKanala="6.23";

	currentZoom=1000;

	Scope.ZoomTo(0,1023,0,currentZoom,false);
	for (int i=0;i<1024;i++)
	{
		MainXArray[i]=float(i);
		MainYArray[i]=0;
	}
	Scope.Channels[ 0 ].Data.SetXYData(MainXArray, MainYArray, 1024);
	Scope.YAxis.AxisLabel.Text =_T("Counts");
	Scope.XAxis.AxisLabel.Text =_T("Channel");


	Scope.ToolBar.Position.CY = 75;

	VCL_Loaded();
	Scope.Hold = false;
//	Scope.Title.Visible = false;
//	Scope.Channels[ 1 ].Name = "Ramp";
	
	Counter = 0;
	cinaKanala="6.23";
	FileName="01_Sample_Exc=300nm_Em=450nm_T=300K_I=10mkA_Fon=200";
	
	combo.AddString("500");
	combo.AddString("1000");
	combo.AddString("2000");
	combo.AddString("4000");
	combo.AddString("8000");
	combo.AddString("16000");
	combo.AddString("32000");
	combo.AddString("64000");
	combostr="64000";
	
	
	
	
	CWinApp* pApp = AfxGetApp();

	//pApp->GetProfileString("decayopt","comport","none");
	FileName=pApp->GetProfileString("decayopt","filename");
	
	comport_ctrl.AddString("COM2");
	comport_ctrl.AddString("COM3");
	comport_ctrl.AddString("COM4");
	comport_ctrl.AddString("COM5");
	comport_ctrl.AddString("COM6");
	comport_ctrl.AddString("COM7");
	comport_ctrl.AddString("COM8");
	comport_ctrl.AddString("COM9");
	
	comport=pApp->GetProfileString("decayopt","comport","none");
	if (comport=="none") comport="COM3";

	SP = new Serial(comport);
	UpdateData(false);
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CArdOscilDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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
		/*CDC* dc;
        dc=GetDC();
        dc->TextOut(20,60,_T("dwgweeeeeeeeeeeeeeeee")); */
		
		CDialog::OnPaint();
	}

}

HCURSOR CArdOscilDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CArdOscilDlg::OnBnClickedPlotButton()
{
	Scope.Channels[ 0 ].Data.SetXYData(MainXArray,MainYArray, 1024 );
}

void CArdOscilDlg::OnBnClickedClearButton()
{
	// TODO: Add your control notification handler code here
	for (int i=0;i<1024;i++) {MainYArray[i]=0;}
	Scope.Channels[ 0 ].Data.Clear();
	Scope.Hold = false;
}

UINT CArdOscilDlg::ThreadFn(LPVOID pParam)
{
    CArdOscilDlg* pThis = (CArdOscilDlg*)pParam;
	pThis->DoIt();
	return 0; 
}

void CArdOscilDlg::OnBnClickedButton1()
{
	UpdateData(true);
	if (!m_ThreadStarted) AfxBeginThread(ThreadFn,this);
}

void CArdOscilDlg::DoIt(void)
{
	SetTimer(ID_TIMER_1,1000,NULL);
	m_StopThread=false;
	m_ThreadStarted=true;
	HWND hWnd=GetSafeHwnd();
	int dataLength = 3256;
	int readResult = 0;
	//readAll = 0;
	char incomingData[3256] = "";
 	byte b1,b2;
    unsigned int B;
	int marker;
	int howLongToSleep=20;
	byte shift=0;
	countmax=atoi(combostr);
	
	if (!(SP->IsConnected()))
	{
		SP->~Serial();
		SP = new Serial(comport);
	}
	

	startTime=CTime::GetCurrentTime();
	time_t stTm = startTime.GetTime();
	//SP->WriteData("7",1);
	//Sleep(2);	
//	int k=0;
	while(!m_StopThread)
	{
		if (m_StopThread) {m_ThreadStarted=false;break;}
		
		readResult=SP->ReadData(incomingData,dataLength);
		if (readResult==0) {Sleep(50); continue;};
		

		
		marker=-1;
		for (int i=0;i<(readResult-4);i++)
		{
			if (incomingData[i]==-1) // "-1" це 0xFF
			{
				if (incomingData[i+1]==-1)
				{
					if (incomingData[i+2]==-1)
					{
						if (incomingData[i+3]==-1)
						{
							marker=i;
							break;
						}
						
					}
				}
			}
		}
		
		if (marker==-1) {Sleep(1); continue;};
		
		shift=marker % 2;

		for (int i=shift; i<readResult-1;i+=2)
		{
			b1=incomingData[i];
			b2=incomingData[i+1];
			B=(b2<<8)|b1;         //(incomingData[i+1]<<8)|incomingData[i];
			B=B>>2; // ділимо на 4
			
			if (B<1024)
			{
				
				if (MainYArray[B]>countmax-1) 
				{
					Beep(500,1000); 
					m_StopThread=true; 
					break;
				}
				else MainYArray[B]++;
				
				
				if (MainYArray[B]>currentZoom) 
				{
					currentZoom*=2;
					
				}
				readAll++;
				
				
			//	m_result.Format(_T("%i"),B);
			}
		}
	
		//t1= clock()-t1;
				
		//	::PostMessage(hWnd,WM_USERMSG,0,0); //повідомлення головному вікну
		Sleep(50);
	}

	stopTime=CTime::GetCurrentTime();
	time_t spTm = stopTime.GetTime();

	timeOfMeas=long(spTm-stTm);

	m_ThreadStarted=false;
	KillTimer(ID_TIMER_1);
}

LRESULT CArdOscilDlg::Mess(WPARAM wParam, LPARAM lParam) //обробка повідомлення від потоку
{
	UpdateData(false);
	return 0L;
}

void CArdOscilDlg::OnBnClickedButton2() // стоп потік
{
	m_StopThread=true;
	//SP->WriteData("8",1);
}

void CArdOscilDlg::OnTimer(UINT_PTR nIDEvent)
{
	UpdateData(true);
	m_number.Format(_T("%d"),readAll);
	readAll=0;
	
	Scope.ZoomTo(0,1023,0,currentZoom,false);
	Scope.Hold = false;
	Scope.Channels[ 0 ].Data.SetXYData(MainXArray, MainYArray, 1024);
	//Scope.PostRefresh();
	UpdateData(false);
	CDialog::OnTimer(nIDEvent);
}

void CArdOscilDlg::OnBnClickedSave()
{
	CFile* pFile = NULL;
	UpdateData(true);
	CString FileNameEx=FileName;
	FileNameEx+=".txt";
	CFileDialog fileDialog(FALSE,NULL,FileNameEx);	//объект класса выбора файла
	int result = fileDialog.DoModal();	//запустить диалоговое окно
	if (result==IDOK)	//если файл выбран
	{
		//AfxMessageBox(fileDialog.GetPathName()); // показать полный путь
		try
            {
               
				pFile = new CFile(fileDialog.GetPathName(),CFile::modeCreate | CFile::modeReadWrite);
			}
        catch (CFileException* pEx)
            {
                TCHAR   szCause[255];          
				pEx->GetErrorMessage(szCause,255); 
				AfxMessageBox(szCause);
            }

		if (pFile != NULL)
            {
				CString vyv; CString tmp;
				
				vyv="time\t"; 

				vyv+=FileName; 	vyv+="_dt="; vyv+=cinaKanala;
				tmp.Format("%i",timeOfMeas); vyv+="_DT="; vyv+=tmp; vyv+="s";
				vyv+='\n';
				pFile->Write(vyv,vyv.GetLength());

				float cinkan=float(atof(cinaKanala));
				
               for(int i=0;i<1024;i++)
					{
						vyv.Empty();
						if (cinkan>0.001) vyv.Format("%.3f\t%.0f\n",MainXArray[i]*cinkan,MainYArray[i]);
						else vyv.Format("%.3f\t%.0f\n",MainXArray[i],MainYArray[i]);
						
						pFile->Write(vyv,vyv.GetLength());
					}




				//pFile->Write("test",4);
				pFile->Close();
                delete pFile;
            }   
	}
}

void CArdOscilDlg::OnBnClickedClear()
{
	
	for (int i=0;i<1024;i++)
	{
		MainYArray[i]=0;
	}
	currentZoom=1000;
	Scope.ZoomTo(0,1023,0,currentZoom,false);
	Scope.Hold = false;
	Scope.Channels[ 0 ].Data.SetXYData(MainXArray, MainYArray, 1024);
	readmax=0;
}

void CArdOscilDlg::OnCbnSelchangeCombo1()
{
	char lpszText[256];
	combo.GetLBText(combo.GetCurSel(), lpszText);
	countmax=atoi(lpszText);
}

void CArdOscilDlg::OnBnClickedCalc()
{
	double a,b,c,d1,d2,x,y;
	UpdateData(true);

	a=float(m_end-m_begin+1); if (a<2) {calcTime=-1; UpdateData(false); return;}
	
	b=0; c=0; d1=0; d2=0;
	double LogArray[1024];

	for (int i=0;i<1024;i++)
	{
		if (MainYArray[i]==0) LogArray[i]=0.0;
		else LogArray[i]=log(MainYArray[i]);
	}

	//for (int i=0;i<1024;i++) // just for test
	//{
	//	LogArray[i]=log(100*exp(-i/200.0));
	//}


	for (int i=m_begin;i<(m_end+1);i++)
	{
		b+=MainXArray[i];
		c+=MainXArray[i]*MainXArray[i];
		d1+=LogArray[i];
		d2+=LogArray[i]*MainXArray[i];
	}


	if ((c-b*b/a)!=0) y=(d2-b*d1/a)/(c-b*b/a); else  {calcTime=-1; UpdateData(false); return;}
	x=d1/a-b/a*y;

	calcTime=float(-1/y);
	
	double tau=calcTime;
	double A=exp(x);
	double xarr[1024],yarr[1024];
	
	for (int i=0;i<1024;i++)
	{
		xarr[i]=i;
		yarr[i]=A*exp(-xarr[i]/tau);
	}
	
	int t=Scope.Channels.Count;
	if (t<2) Scope.Channels.Add();
	Scope.Channels[ 1 ].Data.SetXYData(xarr, yarr, 1024);
	//Scope.Hold = false;
	//Scope.ZoomTo(0,1023,0,currentZoom,false);
	UpdateData(false);
}

void CArdOscilDlg::OnBnClickedClr()
{
	int t=Scope.Channels.Count;
	if (t==2) Scope.Channels.Delete(1);
}

void CArdOscilDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	//CDialog::OnOK();
}

void CArdOscilDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	//CDialog::OnCancel();
}

void CArdOscilDlg::OnBnClickedExit()
{
	UpdateData(true);
	CWinApp* pApp = AfxGetApp();
	pApp->WriteProfileString("decayopt","comport",comport);
	pApp->WriteProfileString("decayopt","filename",FileName);
	
//	pApp->GetProfileString("decayopt","comport",comport);
	CDialog::OnOK();
}

BOOL CArdOscilDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( TranslateAccelerator( m_hWnd, ha, pMsg ) ) return TRUE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CArdOscilDlg::OnSelchangeCombo2()
{
	
	char lpszText[256];
	comport_ctrl.GetLBText(comport_ctrl.GetCurSel(), lpszText);
	//AfxMessageBox(lpszText);
	comport=lpszText;
	
}
