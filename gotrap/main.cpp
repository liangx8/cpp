#include <iostream>
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "go_trap.h"

#define ID_NEW_TRAP		9991
#define ID_PREV_STEP	9992
#define ID_NEXT_STEP	9993

const static char g_szClassName[]="goTrapWindowClass";

static GoTrap *g_pGoTrap;
HWND g_hStatus;
HWND g_hToolbar;
#define TOOLBAR_STYLE  WS_CHILD | WS_BORDER | WS_VISIBLE | TBSTYLE_TOOLTIPS
void createToolbar(HWND hwnd){
	TBBUTTON tbb[3];
	TBADDBITMAP tbab;
	g_hToolbar=CreateWindowEx(0,TOOLBARCLASSNAME,NULL,TOOLBAR_STYLE,0,0,0,0,
				hwnd,(HMENU)NULL,GetModuleHandle(NULL),NULL);
	SendMessage(g_hToolbar,TB_BUTTONSTRUCTSIZE,(WPARAM)sizeof(TBBUTTON),0);
	tbab.hInst = HINST_COMMCTRL;
	tbab.nID = IDB_STD_SMALL_COLOR;
	SendMessage(g_hToolbar, TB_ADDBITMAP, 0, (LPARAM)&tbab);
	ZeroMemory(tbb, sizeof(tbb));
    tbb[0].iBitmap = STD_FILENEW;
    tbb[0].fsState = TBSTATE_ENABLED;
    tbb[0].fsStyle = TBSTYLE_BUTTON;
    tbb[0].idCommand = ID_NEW_TRAP;

    tbb[1].iBitmap = STD_FILEOPEN;
    tbb[1].fsState = TBSTATE_ENABLED;
    tbb[1].fsStyle = TBSTYLE_BUTTON;
    tbb[1].idCommand = ID_PREV_STEP;

    tbb[2].iBitmap = STD_FILESAVE;
    tbb[2].fsState = TBSTATE_ENABLED;
    tbb[2].fsStyle = TBSTYLE_BUTTON;
    tbb[2].idCommand = ID_NEXT_STEP;

    SendMessage(g_hToolbar, TB_ADDBUTTONS, 3 /* sizeof(tbb)/sizeof(TBBUTTON)*/, (LPARAM)&tbb);
}
static int trap_size;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	RECT rcClient;
	switch(msg){
		case WM_LBUTTONDOWN:
			break;
		case WM_CREATE:
			GetClientRect(hwnd, &rcClient);
			{
				int width=rcClient.right-rcClient.left;
				int height=rcClient.bottom-rcClient.top;
				trap_size =  (width>height?height:width)-80;
			}
			break;
		case WM_PAINT:
			hdc=BeginPaint(hwnd,&ps);
			
			g_pGoTrap->paint(hdc,trap_size);
			EndPaint(hwnd,&ps);
			//			UpdateWindow(hwnd);
			break;
		case WM_SIZE:
			SendMessage(g_hStatus,WM_SIZE,0,0);
			SendMessage(g_hToolbar,WM_SIZE,0,0);
			{
				int width=LOWORD(lParam);
				int height=HIWORD(lParam);
				trap_size =  (width>height?height:width)-80;
			}
			InvalidateRect(hwnd,NULL,false);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDM_QUIT:
					PostQuitMessage(0);
					break;
				case ID_NEW_TRAP:
//					SetTimer(hwnd,TIMER_ID,1000,NULL);
					SendMessage(g_hStatus,SB_SETTEXT,0,(LPARAM)"新棋盘");
//					u12pdate_screen(hwnd);
					break;
				case ID_PREV_STEP:
					SendMessage(g_hStatus,SB_SETTEXT,0,(LPARAM)"上一步");
//					KillTimer(hwnd,TIMER_ID);
					break;
				case ID_NEXT_STEP:
					SendMessage(g_hStatus,SB_SETTEXT,0,(LPARAM)"下一步");
//					life->init();
//					InvalidateRect(hwnd,NULL,false);
					break;
			}
			std::cout << LOWORD(wParam) << std::endl;
			break;
		case WM_TIMER:
//			std::cout << "timer -----" << std::endl;
//			if(wParam==TIMER_ID){
//				life->next();
//				InvalidateRect(hwnd,NULL,false);
//			}
			break;
        case WM_CLOSE:
//			KillTimer(hwnd,TIMER_ID);
            DestroyWindow(hwnd);
			break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int WINAPI WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		LPSTR lpCmdLine,
		int nCmdShow){
    WNDCLASSEX wc;
    MSG Msg;

    //Step 1: Registering the Window Class
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = MAKEINTRESOURCE(IDR_MAIN_MENU);
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Step 2: Creating the Window
    HWND hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,//WS_EX_CLIENTEDGE,
        g_szClassName,
        "Life game",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 500,
        NULL, NULL, hInstance, NULL);

    if(hwnd == NULL){
        MessageBox(NULL, "Window Creation Failed!", "Error!",MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }
	GoTrap gt;
	g_pGoTrap=&gt;
// for creat tool bar
	createToolbar(hwnd);
// for creat status bar
	g_hStatus=CreateWindowEx(0,STATUSCLASSNAME,NULL,WS_CHILD|WS_VISIBLE|SBARS_SIZEGRIP,0,0,0,0,
				hwnd,(HMENU)NULL,GetModuleHandle(NULL),NULL);
	int status_widths[]={100,-1};
	SendMessage(g_hStatus,SB_SETPARTS,sizeof(status_widths)/sizeof(int),(LPARAM)status_widths);
	SendMessage(g_hStatus,SB_SETTEXT,0,(LPARAM)"围棋");
    ShowWindow(hwnd, nCmdShow);
	
	

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

}
