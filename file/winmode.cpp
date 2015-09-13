// -*-coding:utf-8-*-
#include <libintl.h>
#include <windows.h>
#include <commctrl.h>
#include <strings.h>
#include <vector>
#include <string>
#include <algorithm>

#include "listview.h"

#define ID_EDIT_PATH   1001
#define ID_BUTTON_FIND 1002
#define ID_HEADER_DATA 1003
#define ID_LIST_VIEW_DATA 1004

#define TOOLBAR_STYLE  (WS_CHILD | WS_BORDER | WS_VISIBLE | TBSTYLE_TOOLTIPS)
#define PATHNAME_MAX_LEN 1024

const char g_szClassName[]="mywindowclass";
extern std::vector<std::pair<DWORD,std::string *>* > dirs;

class MyList:public ListView{

protected:
  size_t rowCount()const {
	return limit;
  }
  size_t colCount()const {
	return 3;
  }
  void fillItem(int row,int col,LV_ITEM *lvi){

	static char buf[1024];
	static char num[50];

	switch(col){
	case 0:
	  sprintf(num,"%d",row);
	  lvi->pszText=num;
	  break;
	case 1:
	  {
		size_t sz=dirs[row]->first;
		char uom=' ';
		if(sz>1024*1024){
		  sz=sz/1024/1024;
		  uom='M';
		} else if (sz > 1024){
		  sz=sz/1024;
		  uom='K';
		}
		sprintf(num,"%d%c",sz,uom);
		lvi->pszText=num;
	  }
	  break;
	case 2:
	  strcpy(buf,dirs[row]->second->c_str());
	  lvi->pszText=buf;
	}

  }
  void fillColumn(int idx,LV_COLUMN *lvc){
	static LPSTR titles[]={gettext("seq"),gettext("size"),gettext("path")};
	static int width[]={66,66,400};
	lvc->pszText=titles[idx];
	lvc->cx=width[idx];
  }
public:
  size_t limit;
  MyList(HWND parent,int Id):ListView(parent,Id){}
  ~MyList(){}
};

MyList *list;
bool cmp(std::pair<unsigned long,std::string *>* l,std::pair<unsigned long,std::string *>* r);
void errorMsg(DWORD errCode);
void proc(const char *fn,LPWIN32_FIND_DATA s);
void walk(const char *top,void (*p)(const char *,LPWIN32_FIND_DATA));
char str_path_buf[PATHNAME_MAX_LEN];
HWND g_hToolbar,g_hEditPath,g_hEditLimit,g_hButtonSearch,/*g_hHeader,*/g_hListView;
bool engage;
void clear_dirs(){
  for(size_t i=0;i<dirs.size();i++){
	delete dirs[i]->second;
	delete dirs[i];
  }
  dirs.clear();
}
DWORD WINAPI search(LPVOID p){
  char buf[20];

  SendMessage(g_hEditLimit,WM_GETTEXT,(WPARAM)20,(LPARAM)buf);
  list->limit=atoi(buf);
  engage=-1;
  clear_dirs();
  walk(str_path_buf,proc);
  if(dirs.empty()){
	engage=0;
	return 0;
  }
  size_t sz=dirs.size();
  list->limit=list->limit > sz ? sz:list->limit;

  printf(gettext("total:%d,limit %d\n"),dirs.size(),list->limit);
  std::sort(dirs.begin(),dirs.end(),cmp);

  if(!list->flash()){
	errorMsg(GetLastError());
  }
  engage=0;
  return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam,LPARAM lParam){
  DWORD threadId;
  HANDLE thr;
  switch(msg){
  case WM_CREATE:
	engage=0;// false
	break;
  case WM_COMMAND:
	switch(LOWORD(wParam)){
	case ID_BUTTON_FIND:
	  if(engage){
		printf("%d,engage ...",__LINE__);
		return DefWindowProc(hwnd,msg,wParam,lParam);
	  }
	  SendMessage(g_hEditPath,WM_GETTEXT,(WPARAM)PATHNAME_MAX_LEN-1,(LPARAM)str_path_buf);
	  thr=CreateThread(
					   NULL,// the handle cannot be inherited
					   0,// the stack size defaults to the same size as that of the primary thread of the process.
					   search,
					   (PVOID)314,// parameter, I push a meanless value.
					   0, // thread runs immediately
					   &threadId);
	  if(thr == NULL){
		errorMsg(GetLastError());
	  }
	  break;
	case ID_EDIT_PATH:	  break;
	// case ID_HEADER_DATA:
	//   printf("%d,header\n",__LINE__); break;
	}
	break;
  case WM_SIZE:
	SendMessage(g_hToolbar,WM_SIZE,0,0);
	{
	  int cx=LOWORD(lParam);
	  int cy=HIWORD(lParam)-20;
	  list->resize(0,30,cx,cy);
	  //	  MoveWindow(g_hListView,0,30,cx,cy,1);
	}
	// {
	//   HD_LAYOUT hdl;
	//   RECT rcParent;
	//   WINDOWPOS wp;
	//   GetClientRect(hwnd,&rcParent);
	//   rcParent.top=23;
	//   hdl.prc=&rcParent;
	//   hdl.pwpos=&wp;
	//   if(!SendMessage(g_hHeader,HDM_LAYOUT,0,(LPARAM) &hdl)){
	// 	errorMsg(GetLastError());
	// 	break;
	//   }
	//   SetWindowPos(g_hHeader,wp.hwndInsertAfter, wp.x, wp.y, wp.cx, wp.cy,wp.flags | SWP_SHOWWINDOW);
	// }

	break;
  case WM_CLOSE:
	DestroyWindow(hwnd);
	break;
  case WM_DESTROY:
	clear_dirs();
	PostQuitMessage(0);
	break;
  default:
	return DefWindowProc(hwnd,msg,wParam,lParam);
  }
  return 0;
}
void createToolbar(HWND parent){
  g_hToolbar = CreateWindowEx(0,TOOLBARCLASSNAME,NULL,TOOLBAR_STYLE,0,
							  0,0,0,parent,(HMENU)NULL,
							  GetModuleHandle(NULL),NULL);
  g_hEditPath = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","c:/users",
							  WS_CHILD | WS_VISIBLE|ES_AUTOHSCROLL ,
							  0,0,300,24,g_hToolbar,(HMENU)ID_EDIT_PATH,GetModuleHandle(NULL),NULL);
  if(g_hEditPath == NULL){
	errorMsg(GetLastError());
	return;
  }
  g_hEditLimit = CreateWindowEx(WS_EX_CLIENTEDGE,"EDIT","10",
							  WS_CHILD | WS_VISIBLE|ES_AUTOHSCROLL ,
							  304,0,80,24,g_hToolbar,(HMENU)ID_EDIT_PATH,GetModuleHandle(NULL),NULL);
  if(g_hEditPath == NULL){
	errorMsg(GetLastError());
	return;
  }
  g_hButtonSearch=CreateWindowEx(WS_EX_CLIENTEDGE,"BUTTON",gettext("list"),WS_CHILD|WS_VISIBLE,
							  390,0,80,24,g_hToolbar,(HMENU)ID_BUTTON_FIND,GetModuleHandle(NULL),NULL);
  if(g_hButtonSearch == NULL){
	errorMsg(GetLastError());
	return;
  }
}
int winmode(HINSTANCE hInstance, HINSTANCE hPrevInstance,
LPSTR lpCmdLine, int nCmdShow){
  MSG msg;
  WNDCLASSEX wc;
  HWND hwnd;
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.style =0;
  wc.lpfnWndProc=WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra =0;
  wc.hInstance = hInstance;
  wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor = LoadCursor(NULL,IDC_ARROW);
  wc.hbrBackground =(HBRUSH)(COLOR_WINDOW+1);
  wc.lpszMenuName= NULL;
  wc.lpszClassName = g_szClassName;
  wc.hIconSm = LoadIcon(NULL,IDI_APPLICATION);
  if(!RegisterClassEx(&wc)){
	errorMsg(GetLastError());
	return 0;
  }
  hwnd=CreateWindowEx(WS_EX_CLIENTEDGE,
					  g_szClassName,
					  gettext("Find the largest file in the directory."),
					  WS_OVERLAPPEDWINDOW,
					  CW_USEDEFAULT, CW_USEDEFAULT, 640, 480,
					  NULL, NULL, hInstance, NULL);
  if(hwnd==NULL){
	errorMsg(GetLastError());
	return 0;
  }
  InitCommonControls();
  createToolbar(hwnd);

  /************************************************************************/
  // ListView Control 构造开始
  list=new MyList(hwnd,ID_LIST_VIEW_DATA);
  if(!list->initHeader()){
	errorMsg(GetLastError());
	return 0;
  }
  // g_hListView=CreateWindowEx(WS_EX_CLIENTEDGE,WC_LISTVIEW, "",
  // 							 WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_EDITLABELS,
  // 							 0,0,100,100,
  // 							 hwnd,(HMENU)ID_LIST_VIEW_DATA,GetModuleHandle(NULL),NULL);
  // LV_COLUMN column;
  // ZeroMemory(&column,sizeof(LV_COLUMN));
  // column.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;

  // column.pszText = gettext("seq");
  // column.cx=66;
  // SendMessage(g_hListView,LVM_INSERTCOLUMN,0,(LPARAM)&column);
  // column.pszText = gettext("size");
  // column.cx=66;
  // SendMessage(g_hListView,LVM_INSERTCOLUMN,1,(LPARAM)&column);
  // column.pszText = gettext("path");
  // column.cx=400;
  // SendMessage(g_hListView,LVM_INSERTCOLUMN,2,(LPARAM)&column);
  // ListView Control 结束
  /************************************************************************/
  ShowWindow(hwnd,nCmdShow);
  UpdateWindow(hwnd);
  while(GetMessage(&msg,NULL,0,0)>0){
	TranslateMessage(&msg);
	DispatchMessage(&msg);
  }
  return 0;
}
