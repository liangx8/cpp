#include <stdio.h>
#include <wchar.h>
#include <locale.h>
int main(){
  setlocale(LC_ALL,"");
  wprintf(L"显示字符\n");
  for(int i=0;i<256;i++){
    wprintf(L"\033[31m%02x\033[0m \033[32m00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\033[0m\n",i);
    for(int x=0;x<16;x++){
      wprintf(L"\033[33m%02x\033[0m ",x*16);
      for(int y=0;y<16;y++){
	wchar_t c;
	if (y%2){
	  wprintf(L"\033[30;42m");
	} else {
	  wprintf(L"\033[30;43m");
	}
	c=i*256+x*16+y;
	wprintf(L"%lc\033[0m ",c);
      }
      wprintf(L"\n");
    }
    wprintf(L"\n");
  }
 }
/*
0c ䷀ ䷁ ䷂ ䷃ ䷄ ䷅ ䷆ ䷇ ䷈ ䷉ ䷊ ䷋ ䷌ ䷍ ䷎ ䷏ 
0d ䷐ ䷑ ䷒ ䷓ ䷔ ䷕ ䷖ ䷗ ䷘ ䷙ ䷚ ䷛ ䷜ ䷝ ䷞ ䷟ 
0e ䷠ ䷡ ䷢ ䷣ ䷤ ䷥ ䷦ ䷧ ䷨ ䷩ ䷪ ䷫ ䷬ ䷭ ䷮ ䷯ 
0f ䷰ ䷱ ䷲ ䷳ ䷴ ䷵ ䷶ ䷷ ䷸ ䷹ ䷺ ䷻ ䷼ ䷽ ䷾ ䷿ 
 */
