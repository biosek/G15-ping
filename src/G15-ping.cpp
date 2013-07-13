#include <windows.h>
#include "EZ_LCD.h"
#include "G15-ping.h"    // Used to quicken access to our .data defines etc.
#include <time.h>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <string>

using namespace std;

#define  WINDOWS_LEAN_AND_MEAN

DWORD WINAPI LCDDisplaySystem(LPVOID);  // LCD Display function      
DWORD WINAPI DllReadLoop(LPVOID);  // DLL ReadLoop function
int min = 50;
int max = 0;
int avg = 0;
int cnt = 0;

int APIENTRY WinMain(HINSTANCE hInstance,                
                     HINSTANCE /*hPrevInstance*/,
                     LPSTR     /*lpCmdLine*/,
                     int       /*nCmdShow*/)
{
	hInst = hInstance; 
     DllReadLoopThread = ::CreateThread(NULL,0,DllReadLoop,NULL,0,DllReadLoopID); //This read is used by our program to scan the memory of our target and our DLL.
     DisplayLCDThread = ::CreateThread(NULL,0,LCDDisplaySystem,NULL,0,DisplayLCDID); // Used by our LCD Display function

	   while (ExitApplet != 1)
	   {
		   ::Sleep(200);                
	   };

	   ::Sleep(2000);   // Just a little kill time so we can display our lcd text about the shut down. This is not needed but I use it so people dont think our proggies crashed.
	   ::CloseHandle(DllReadLoopThread); // Closes the ReadLoop Thread.
	   ::CloseHandle(DisplayLCDThread);  // Closes the LCD Thread.
}

DWORD WINAPI DllReadLoop(LPVOID)
{
	while (ExitApplet !=1)
	{
		time_t rt; 
		struct tm * ti = 0; 
		char bt[35]; 
		time(&rt); 
        ti = localtime(&rt);
		strftime(bt,80,"%X",ti);
        strcpy(TimeString, bt);

		time(&rt); 
        ti = localtime(&rt);
		strftime(bt,80,"%x",ti);
        strcpy(DayString, bt);

		HANDLE child_output_read;
		HANDLE child_output_write;
		CHAR buffer[1024];
		DWORD bytes_read;
		PROCESS_INFORMATION process_info;
		STARTUPINFOA startup_info;
		SECURITY_ATTRIBUTES security_attributes;
 
		// Set the security attributes for the pipe handles created
		security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
		security_attributes.bInheritHandle = TRUE;
		security_attributes.lpSecurityDescriptor = NULL;
		CreatePipe(&child_output_read, &child_output_write, &security_attributes, 0);
 
		// Create the child process
		ZeroMemory(&process_info, sizeof(PROCESS_INFORMATION));
		ZeroMemory(&startup_info, sizeof(STARTUPINFO));
		startup_info.cb = sizeof(STARTUPINFO);
		startup_info.hStdOutput = child_output_write;
		startup_info.hStdError = child_output_write;
		startup_info.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		startup_info.wShowWindow = SW_HIDE;

        CreateProcessA(NULL, (char*)"ping seznam.cz -n 1", NULL, NULL, TRUE, 0, NULL, NULL, &startup_info, &process_info);
		WaitForSingleObject( process_info.hProcess, INFINITE );
		CloseHandle( process_info.hProcess );
		CloseHandle( process_info.hThread );

		// Read the message from the child process
		ReadFile( child_output_read, buffer, sizeof(buffer), &bytes_read, NULL);
		buffer[bytes_read] = 0;
		string buff = string(buffer);
		string time = buff.substr(buff.find("time=") + 5,buff.find("ms")-buff.find("time=") - 5);
		
        int itime = atoi(time.c_str());
        strcpy(ping1, ping2);
        strcpy(ping2, ping3);
		if(itime != 0)
		{
			imin = imin > itime ? itime : imin;
			imax = imax < itime ? itime : imax;
			icnt++;
			isum += itime;
			iavg = isum/icnt;
            sprintf(PingStats,"min:%d max:%d avg:%d", imin, imax, iavg);
		
            sprintf(ping3,"ping time=%dms",itime);
		}
		else
		{
            sprintf(PingStats,"min:%d max:%d avg:%d", imin, imax, iavg);
            sprintf(ping3,"ping timed out",itime);
		}
		
		::Sleep(1000);
	}
	return 0;
}

DWORD WINAPI LCDDisplaySystem( LPVOID )
{
    pEzlcd = new CEzLcd();

    hRes = pEzlcd->InitYourself(LCDIdentStr);

    if (hRes != S_OK)
    {
        delete pEzlcd;
        return 0 ;
    }

	HANDLE Page0Text1 = pEzlcd->AddText(LG_STATIC_TEXT, LG_CWFONT, DT_LEFT, 160);
    pEzlcd->SetOrigin(Page0Text1, 0, 0);
	HANDLE Page0Text2 = pEzlcd->AddText(LG_STATIC_TEXT, LG_CWFONT, DT_LEFT, 160);
    pEzlcd->SetOrigin(Page0Text2, 0, 16);
    HANDLE Page0Text3 = pEzlcd->AddText(LG_STATIC_TEXT, LG_SMALL, DT_CENTER, 160);
    pEzlcd->SetOrigin(Page0Text3, 0, 32);

	HANDLE Ping1 = pEzlcd->AddText(LG_STATIC_TEXT, LG_SMALL, DT_LEFT, 160);
    pEzlcd->SetOrigin(Ping1, 80, 0);
	HANDLE Ping2 = pEzlcd->AddText(LG_STATIC_TEXT, LG_SMALL, DT_LEFT, 160);
    pEzlcd->SetOrigin(Ping2, 80, 11);
	HANDLE Ping3 = pEzlcd->AddText(LG_STATIC_TEXT, LG_SMALL, DT_LEFT, 160);
    pEzlcd->SetOrigin(Ping3, 80, 22);

	while (1)
	{  
		  pEzlcd->SetVisible(Page0Text1,FALSE);
		  pEzlcd->SetVisible(Page0Text2,FALSE);
		  pEzlcd->SetVisible(Page0Text3,FALSE);
		  pEzlcd->SetVisible(Ping1,FALSE);
		  pEzlcd->SetVisible(Ping2,FALSE);
		  pEzlcd->SetVisible(Ping3,FALSE);
		   
		  if (PageDisplay==0)
		  {
   		     pEzlcd->ModifyControlsOnPage(PageDisplay);
			 pEzlcd->ShowPage(PageDisplay);

			 pEzlcd->SetVisible(Page0Text1,TRUE);
	   		 pEzlcd->SetText(Page0Text1,TimeString);
			 pEzlcd->SetVisible(Page0Text2,TRUE);
	   		 pEzlcd->SetText(Page0Text2,DayString);
			 pEzlcd->SetVisible(Page0Text3,TRUE);
	   		 pEzlcd->SetText(Page0Text3,PingStats);
			 pEzlcd->SetVisible(Ping1,TRUE);
	   		 pEzlcd->SetText(Ping1,ping1);
			 pEzlcd->SetVisible(Ping2,TRUE);
	   		 pEzlcd->SetText(Ping2,ping2);
			 pEzlcd->SetVisible(Ping3,TRUE);
	   		 pEzlcd->SetText(Ping3,ping3);
		  }
		  else if (PageDisplay==5)
		  {
		     pEzlcd->ModifyControlsOnPage(PageDisplay);
	         pEzlcd->ShowPage(PageDisplay);
			 pEzlcd->SetVisible(Page0Text3,TRUE);
			 pEzlcd->SetText(Page0Text3, sysmsg);
		  }

          if (pEzlcd->IsConnected())
		  {
			if (pEzlcd->ButtonReleased(LG_BUTTON_1)) //ButtonTriggered
			{
              PageDisplay=0;
			} 
			else if (::pEzlcd->ButtonIsPressed(LG_BUTTON_3) && ::pEzlcd->ButtonIsPressed(LG_BUTTON_4))
			{
			 PageDisplay=5;
             strcpy(sysmsg, "Shutting Down. in 2 seconds");
			 ExitApplet=1; // Exit Applet
			}
		  }
	      ::Sleep(50); // pause loop so we dont cause a lockup 50ms
		  pEzlcd->Update();


	}

delete pEzlcd;


return 0;
}
