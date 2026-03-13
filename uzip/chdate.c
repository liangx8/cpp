#include <wchar.h>
#include <stdio.h>
const wchar_t mname=L'㋀';
const wchar_t dname=L'㏠';
void print_date(int year,int month,int mday)
{
    if(mday>0){
        mday--;
    }
    wprintf(L"%4d年%lc%lc ",year,mname+month,dname+mday);
}