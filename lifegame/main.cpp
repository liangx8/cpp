#include <iostream>
#include <windows.h>
#include <commctrl.h>
#include "resource.h"
#include "life_pattern.h"

#define TIMER_ID 10001
const static char g_szClassName[]="lifeGameWindowClass";

static LifePattern *life;
HWND g_hStatus;
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch(msg){
		case WM_LBUTTONDOWN:
			life->new_life(LOWORD(lParam),HIWORD(lParam));
			InvalidateRect(hwnd,NULL,false);
			break;
		case WM_CREATE:
			break;
		case WM_PAINT:
			hdc=BeginPaint(hwnd,&ps);
			life->paint(hdc);
			EndPaint(hwnd,&ps);
//			UpdateWindow(hwnd);
			break;
		case WM_SIZE:
			SendMessage(g_hStatus,WM_SIZE,0,0);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)){
				case IDM_EXIT:
					PostQuitMessage(0);
					break;
				case IDM_OPERATION_START:
					SetTimer(hwnd,TIMER_ID,1000,NULL);
					SendMessage(g_hStatus,SB_SETTEXT,0,(LPARAM)"生命游戏");
//					u12pdate_screen(hwnd);
					break;
				case IDM_OPERATION_STOP:
					SendMessage(g_hStatus,SB_SETTEXT,0,(LPARAM)"生命游戏 - 停止");
					KillTimer(hwnd,TIMER_ID);
					break;
				case IDM_OPERATION_RESET:
					life->init();
					InvalidateRect(hwnd,NULL,false);
					break;
			}
			break;
		case WM_TIMER:
//			std::cout << "timer -----" << std::endl;
			if(wParam==TIMER_ID){
				life->next();
	//			SendMessage(hwnd,WM_PAINT,wParam,lParam);
	//			UpdateWindow(hwnd);
				InvalidateRect(hwnd,NULL,false);
			}
			break;
        case WM_CLOSE:
			KillTimer(hwnd,TIMER_ID);
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
	LifePattern lp(SIZE,hwnd);
	lp.init();
	life=&lp;
// for create status bar
	g_hStatus=CreateWindowEx(0,STATUSCLASSNAME,NULL,WS_CHILD|WS_VISIBLE|SBARS_SIZEGRIP,0,0,0,0,
				hwnd,(HMENU)NULL,GetModuleHandle(NULL),NULL);
	int status_widths[]={100,-1};
	SendMessage(g_hStatus,SB_SETPARTS,sizeof(status_widths)/sizeof(int),(LPARAM)status_widths);
	SendMessage(g_hStatus,SB_SETTEXT,0,(LPARAM)"生命游戏");
    ShowWindow(hwnd, nCmdShow);
	
	

    // Step 3: The Message Loop
    while(GetMessage(&Msg, NULL, 0, 0) > 0){
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

}
