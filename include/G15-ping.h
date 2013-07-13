/*****************************************
  Setup the defines for our Systray Menu
******************************************/
#define IDM_TRAYEXIT      3 
#define IDM_NULL          4
#define IDM_TRAYABOUT     5
/*****************************************
                Structures
******************************************/
CEzLcd*                      pEzlcd;       // Defines our LCD structure
NOTIFYICONDATA                 tnid;       // Struct used by the systray icon
/******************************************
                Global variables
******************************************/
int imin = 50;
int imax = 0;
int iavg = 0;
int icnt = 0;
int isum = 0;
int pos = 1;
int                          ExitApplet=0; // Used to Flag Termination of all Internal Loop Functions. aka kill switch.
HINSTANCE                    hInst; // Store our HINSTACE for global access (incase we need it).
HWND                         hDlg;  // Store our Dialogs HWND (HANDLE) incase we need it.
HRESULT                      hRes = S_OK;
/*****************************************
     Thread Control System & Stuff
******************************************/
HANDLE                      DllReadLoopThread,DisplayLCDThread;
LPDWORD                     DisplayLCDID=0,MAINIpId=0,DllReadLoopID=0;

/*****************************************
       LCD PAGE DISPLAY STUFF
******************************************/
char                         LCDIdentStr[]="G15-ping"; // Applet Ident Name (shown on page flip and in the manager)
int                          PageDisplay=0; // Used to Pick which pages to show
char                         sysmsg[260]="0";  // store our system messages for the display
char                         TimeString[35]; // just a buffer for our text input
char                         DayString[35]; // just a buffer for our text input
char                         PingStats[35]; // just a buffer for our text input
char                         ping1[4096]; // just a buffer for our text input
char                         ping2[35]; // just a buffer for our text input
char                         ping3[35]; // just a buffer for our text input
