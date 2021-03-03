// INCLUDES /////////////////////////////////////////////////////////////////

#include<afxwin.h>
#include<io.h>
#include<mmsystem.h>
#include<math.h>
#include"texture.h"

// GLOBALS //////////////////////////////////////////////////////////////////

HWND hWnd;

// PROTOTYPES ///////////////////////////////////////////////////////////////

class CxFrameWnd:public CFrameWnd
	{
	public:
		CxFrameWnd();
		~CxFrameWnd();
	protected:
		CWindowDC* VideoDisplay;
		CDC* DoubleBuffer;
		CBitmap* DoubleBufferBmp;
		BOOL RedrawActive;
		struct Quadril
			{
			UINT x1,y1;
			UINT x2,y2;
			UINT x3,y3;
			UINT x4,y4;
			};
		Quadril QPos[5];
		float xx[1000][2];
    	float yy[1000][2];
    	CBitmap* Bmp;
		CDC* Temp;
					    	    	    				
		void Initialize(void);
		void RenderBitmap(UINT BmpID,Quadril Quad);
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg void OnRButtonDown(UINT nID,CPoint point);
		afx_msg void OnClose();
		DECLARE_MESSAGE_MAP();
	};

/////////////////////////////////////////////////////////////////////////////

class CxWinApp:public CWinApp
	{
	public:
		virtual BOOL InitInstance();
	};

// FUNCTIONS ////////////////////////////////////////////////////////////////

CxFrameWnd::CxFrameWnd()
	{
	const char* GameScreen;
	
	GameScreen=AfxRegisterWndClass(CS_BYTEALIGNWINDOW|
								   CS_BYTEALIGNCLIENT|
								   CS_CLASSDC,
	   							   LoadCursor(AfxGetInstanceHandle(),
	   							   			  IDC_ARROW),
								   (HBRUSH)GetStockObject(BLACK_BRUSH),
								   LoadIcon(AfxGetInstanceHandle(),
								   			  "ProgramIcon"));
	Create(GameScreen,
		   "Windows Video Game",
		   WS_POPUP,
		   rectDefault);
	
	hWnd=m_hWnd;
	
	if(!(SetTimer(TMR_INITIALIZE,100,NULL)))
		{
		MessageBox("Timer resource not available.\nClose some open applications or reboot.",
				   "Fatal Error - Aborting",
				   MB_OK);
		SendMessage(WM_CLOSE);
		}		
	}

CxFrameWnd::~CxFrameWnd()
	{
	KillTimer(TMR_MAINLOOP);
	KillTimer(TMR_FPSCOUNT);
	
	VideoDisplay->SelectStockObject(NULL_PEN);
	delete VideoDisplay;
	
	DoubleBuffer->SelectStockObject(NULL_PEN);
	delete DoubleBuffer;
	
	DoubleBufferBmp->Detach();
	DoubleBufferBmp->DeleteObject();
	delete DoubleBufferBmp;
	
	Temp->SelectStockObject(NULL_BRUSH);
	delete Temp;
	
	Bmp->Detach();
	Bmp->DeleteObject();
	delete Bmp;
	}

void CxFrameWnd::Initialize(void)		
	{
	//srand(UINT(timeGetTime()));
	
	VideoDisplay=new CWindowDC(this);
	
	DoubleBuffer=new CDC;             
	DoubleBuffer->CreateCompatibleDC(VideoDisplay);
	
	DoubleBufferBmp=new CBitmap;
	DoubleBufferBmp->CreateCompatibleBitmap(VideoDisplay,800,600);
	DoubleBuffer->SelectObject(DoubleBufferBmp);

	Bmp=new CBitmap;
	Bmp->LoadBitmap("TextureBmp");
	
	Temp=new CDC;
	Temp->CreateCompatibleDC(VideoDisplay);
	Temp->SelectObject(Bmp);
	
	QPos[0].x1=200;
    QPos[0].y1=200;
    QPos[0].x2=250;
    QPos[0].y2=180;
    QPos[0].x3=200;
    QPos[0].y3=250;
    QPos[0].x4=250;
    QPos[0].y4=230;
	
	QPos[1].x1=250;
    QPos[1].y1=180;
    QPos[1].x2=300;
    QPos[1].y2=200;
    QPos[1].x3=250;
    QPos[1].y3=230;
    QPos[1].x4=300;
    QPos[1].y4=250;
	
	QPos[2].x1=200;
    QPos[2].y1=200;
    QPos[2].x2=250;
    QPos[2].y2=220;
    QPos[2].x3=200;
    QPos[2].y3=250;
    QPos[2].x4=250;
    QPos[2].y4=270;
	
	QPos[3].x1=250;
    QPos[3].y1=220;
    QPos[3].x2=300;
    QPos[3].y2=200;
    QPos[3].x3=250;
    QPos[3].y3=270;
    QPos[3].x4=300;
    QPos[3].y4=250;
	
	QPos[4].x1=200;
    QPos[4].y1=200;
    QPos[4].x2=150;
    QPos[4].y2=180;
    QPos[4].x3=250;
    QPos[4].y3=180;
    QPos[4].x4=200;
    QPos[4].y4=160;
    }

void CxFrameWnd::OnTimer(UINT nIDEvent)
	{
	static UINT frame=0;
	char s[255];
					
	switch(nIDEvent)
		{
		case(TMR_INITIALIZE):
			KillTimer(TMR_INITIALIZE);
			Initialize();
			RedrawActive=FALSE;
			if(!(SetTimer(TMR_MAINLOOP,1000/DESFPS,NULL)))
				{
				MessageBox("Timer resource not available.\nClose some open applications or reboot.",
						   "Fatal Error - Aborting",
						   MB_OK);
				SendMessage(WM_CLOSE);
				}		
			if(!(SetTimer(TMR_FPSCOUNT,1000,NULL)))
				{
				MessageBox("Timer resource not available.\nClose some open applications or reboot.",
						   "Fatal Error - Aborting",
						   MB_OK);
				SendMessage(WM_CLOSE);
				}		
			break;
		case(TMR_MAINLOOP):
			if(RedrawActive)
				return;
			RedrawActive=TRUE;
            DoubleBuffer->BitBlt(0,0,800,600,NULL,0,0,BLACKNESS);
			QPos[4].x2+=1;
			QPos[4].y2-=1;
			QPos[4].x4+=1;
			QPos[4].y4-=1;
			RenderBitmap(0,QPos[0]);   
			RenderBitmap(0,QPos[1]);
			RenderBitmap(0,QPos[2]);
			RenderBitmap(0,QPos[3]);
			RenderBitmap(0,QPos[4]);
		    VideoDisplay->BitBlt(0,0,800,600,DoubleBuffer,0,0,SRCCOPY);
		    frame++;
		    RedrawActive=FALSE;
		    break;
		case(TMR_FPSCOUNT):
			sprintf(s,"%d",frame);
			VideoDisplay->TextOut(0,0,s,2);
	        frame=1;
			break;
		}
	}

void CxFrameWnd::RenderBitmap(UINT BmpID,Quadril Quad)
	{
	UINT c,c1,c2;
	
	DoubleBuffer->BitBlt(0,50,100,100,Temp,0,0,SRCCOPY);
		
	float x1,y1;
    x1=Quad.x1;
    y1=Quad.y1;
    float x2,y2;
    x2=Quad.x2;
    y2=Quad.y2;
    float x3,y3;
    x3=Quad.x3;
    y3=Quad.y3;
    float x4,y4;
    x4=Quad.x4;
    y4=Quad.y4;
    
    /*VideoDisplay->MoveTo(int(x1),int(y1));
    VideoDisplay->LineTo(int(x2),int(y2));
    VideoDisplay->LineTo(int(x3),int(y3));
    VideoDisplay->LineTo(int(x4),int(y4));
    VideoDisplay->LineTo(int(x1),int(y1));*/
    
    float dx,dy;
    dx=(x2-x1)/100;
    dy=(y2-y1)/100;
    float x,y;
    x=x1;
    y=y1;
    for(c=0;c<100;c++)
    	{
    	xx[c][0]=x;
    	yy[c][0]=y;
    	x+=dx;
    	y+=dy;
    	}
    dx=(x4-x3)/100;
    dy=(y4-y3)/100;
    x=x3;
    y=y3;
    for(c=0;c<100;c++)
    	{
    	xx[c][1]=x;
    	yy[c][1]=y;
    	x+=dx;
    	y+=dy;
    	}
    for(c1=0;c1<100;c1++)
    	{
    	dx=(xx[c1][1]-xx[c1][0])/100;
    	dy=(yy[c1][1]-yy[c1][0])/100;
    	x=xx[c1][0];
    	y=yy[c1][0];
    	for(c2=0;c2<100;c2++)
    		{
    		DoubleBuffer->SetPixel(int(x),int(y),DoubleBuffer->GetPixel(c1,c2+50));
    		x+=dx;
    		y+=dy;
    		}
    	}
	}

void CxFrameWnd::OnRButtonDown(UINT nID,CPoint point)
	{
	SendMessage(WM_CLOSE);
	}

void CxFrameWnd::OnClose()
	{
	if(MessageBox("Are you sure you want to quit?",
				  "Query",
				  MB_YESNO|MB_ICONQUESTION)==IDYES)
		DestroyWindow();
	}

BEGIN_MESSAGE_MAP(CxFrameWnd,CFrameWnd)
	ON_WM_TIMER()
	ON_WM_RBUTTONDOWN()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CxWinApp::InitInstance()
	{
	m_pMainWnd=new CxFrameWnd();
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED|m_nCmdShow);
	m_pMainWnd->UpdateWindow();
	return TRUE;
	}

// MAIN /////////////////////////////////////////////////////////////////////

CxWinApp Game1;
