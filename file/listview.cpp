// -*-coding:utf-8-*-
#include <stdio.h>
#include "listview.h"

ListView::ListView(HWND parent,int Id){
  hList=CreateWindowEx(WS_EX_CLIENTEDGE,WC_LISTVIEW, "",
					   WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_EDITLABELS,
					   0,0,100,100,
					   parent,(HMENU)Id,GetModuleHandle(NULL),NULL);
}
LRESULT ListView::initHeader(){
  size_t s=colCount();

  LV_COLUMN column;
  ZeroMemory(&column,sizeof(LV_COLUMN));
  column.mask=LVCF_TEXT|LVCF_WIDTH|LVCF_SUBITEM;
  for(size_t i=0;i<s;i++){
	fillColumn(i,&column);
	if(SendMessage(hList,LVM_INSERTCOLUMN,i,(LPARAM)&column)==-1){
	  return FALSE;
	}
  }
  return TRUE;
}
LRESULT ListView::flash(){
  if(!SendMessage(hList,LVM_DELETEALLITEMS,0,0)){

	return FALSE;
  }
  size_t total=rowCount();
  int cols=colCount();
  LV_ITEM lvi;
  lvi.mask=LVIF_TEXT|LVIF_STATE;
  lvi.state=0;
  lvi.stateMask=0;
  for(size_t i=0;i<total;i++){
	for(int j=0;j<cols;j++){
	  fillItem(i,j,&lvi);
	  lvi.iItem=i;
	  lvi.iSubItem=j;
	  if(j==0){
		SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&lvi);
	  } else {
		SendMessage(hList,LVM_SETITEM,0,(LPARAM)&lvi);
	  }

	}

  }
  return TRUE;

}

LRESULT ListView::resize(int x,int y, int cx,int cy){
  return MoveWindow(hList,x,y,cx,cy,1);
}
