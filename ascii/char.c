#include <stdio.h>
#include <wchar.h>
#include <locale.h>
const char *hex="0123456789ABCDEF";
int isPrintable(wchar_t c){
  return c>=0x20;
}
int isOne(wchar_t c){
  switch(c>>8){
  case 0x25: // 单字符制表符
  case 0x28: // 盲文
  case 0x21:
  case 0x22:
  case 0x23:
  case 0x24:
	return 1;
  }
  return c<256;
}
//int hexstr2i(const char *);
int hexstr2i(const char *p){
  int x;
  int i=0;
  char c;
  x=0;
  while(1){
	c=p[i];
	if (c=='\0') break;
	x=x*16;
	if (c < 0x30 ) return -1;
	if (c > 0x66 ) return -1;
	if (c > 0x46 && c < 0x41) return -1;
	if(c>0x60 && c < 0x67) c= c-0x20;
	for (int j=0;j<16;j++){
	  if(hex[j]== c){
		x += j;
	  }
	}
	i ++;
  }
  if (x > 255) return -1;
  return x;
}
void show_section(int i){
    wprintf(L"\033[31m%02x\033[0m \033[32m00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\033[0m\n",i);
    for(int x=0;x<16;x++){
      wprintf(L"\033[33m%02x\033[0m ",x*16);
      for(int y=0;y<16;y++){
		wchar_t c;
		wprintf(L"\033[0;34;47m");
		c=i*256+x*16+y;
		if (isPrintable(c)){
		  if (isOne(c))
			wprintf(L"%lc\033[0m  ",c);
		  else
			wprintf(L"%lc\033[0m ",c);
		}else
		  wprintf(L"NA\033[0m ");
      }
      wprintf(L"\n");
    }
    wprintf(L"\n");
}
int main(int argc,char **argv){
  setlocale(LC_ALL,"");

  int section;
  if(argc>1){
	section=hexstr2i(argv[1]);
	if(section>=0){
	  show_section(section);
	} else {
	  wprintf(L"wrong number, must in range 0x00 ~ 0xff\n");
	}
	return 0;
  }

  for(int i=0;i<256;i++){
	show_section(i);
  }
}
/*
0c ䷀ ䷁ ䷂ ䷃ ䷄ ䷅ ䷆ ䷇ ䷈ ䷉ ䷊ ䷋ ䷌ ䷍ ䷎ ䷏ 
0d ䷐ ䷑ ䷒ ䷓ ䷔ ䷕ ䷖ ䷗ ䷘ ䷙ ䷚ ䷛ ䷜ ䷝ ䷞ ䷟ 
0e ䷠ ䷡ ䷢ ䷣ ䷤ ䷥ ䷦ ䷧ ䷨ ䷩ ䷪ ䷫ ䷬ ䷭ ䷮ ䷯ 
0f ䷰ ䷱ ䷲ ䷳ ䷴ ䷵ ䷶ ䷷ ䷸ ䷹ ䷺ ䷻ ䷼ ䷽ ䷾ ䷿ 
 */
