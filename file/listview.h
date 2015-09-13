// -*-coding:utf-8-*-
#ifndef LISTVIEW_H
#define LISTVIEW_H
#include <windows.h>
#include <commctrl.h>
class ListView{
 private:
  HWND hList;
 protected:
  virtual size_t rowCount()const = 0;
  virtual size_t colCount()const = 0;
  virtual void fillColumn(int idx,LV_COLUMN *lvc)=0;
  virtual void fillItem(int rowx,int col,LV_ITEM *lvi) = 0;
 public:
  ListView(HWND parent,int Id);
  virtual ~ListView(){}

  LRESULT resize(int x,int y,int cx,int cy);
  LRESULT initHeader();
  LRESULT flash();
};

#endif
