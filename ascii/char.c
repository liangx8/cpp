// 😇
// ♈♉♊♋♌♍♎♏
// 🔋 🔌⚡
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>
#include <unistd.h>
#include <iconv.h>
#include <stdint.h>


uint32_t abcd(uint32_t t)
{
	return t/7;
}

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
  case 0xf0:
  case 0xf1:
  case 0xf2:
  case 0xf3:
  case 0xf4:
  case 0xf5:
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
void show_section(int i,int (*cvt)(wchar_t,wchar_t[])){
  int cnt;
  wchar_t xx[8];
    wprintf(L"\033[31m%04x\033[0m \033[32m00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F\033[0m\n",i);
    for(int x=0;x<16;x++){
      wprintf(L"\033[33m%04x\033[0m ",x*16);
      for(int y=0;y<16;y++){
	wchar_t c;
	wprintf(L"\033[0;34;47m");
	c=i*256+x*16+y;
	cnt=cvt(c,xx);
	for(int y=0;y<cnt;y++){
	  wprintf(L"%lc",xx[y]);
	}
	wprintf(L"\033[0m ");
      }
      wprintf(L"\n");
    }
    wprintf(L"\n");
}
void decode_and_print(const char *what)
{
  iconv_t uni=iconv_open("UNICODE","UTF-8");
  char *obuf=malloc(100);
  char *p=obuf;
  size_t inlen=strlen(what);
  size_t outlen=100;
  if(obuf == NULL){
    wprintf(L"error 1\n");
  }
  if(iconv(uni,(char **)&what,&inlen,&p,&outlen) < 0){
    wprintf(L"convert error\n");
  }
  wprintf(L"UNICODE:");
  for(char *i=obuf;i!=p;i++){
    wprintf(L"0x%x ",*i);
  }
  wprintf(L"\n");
  free(obuf);
}
int ok(wchar_t in,wchar_t buf[])
{
  int size=1;
  if(isPrintable(in)){
    buf[0]=in;
  } else {
    buf[0]=L'?';
  }
  if(isOne(in)){
    buf[1]=L' ';
    size=2;
  }

  return size;
}

void symbol(void)
{
  wprintf(L"Greek alphabet(希腊字母) UNICDE UPPERCASE[U+0391 ~ U+03A9]\n");
  show_section(3,ok);
  wprintf(L"Miscellaneous Symbols\n");
  show_section(0x26,ok);
}
void ext(int sect)
{
  sect = sect * 256;
  for(int i=0;i<256;i++){
    show_section(sect+i,ok);
  }
}
int main(int argc,char **argv){
  setlocale(LC_ALL,"");

  int section;
  int opt;

  while((opt=getopt(argc,argv,"e:gd:t:")) != -1){
    switch(opt){
    case 'd':
      section=hexstr2i(optarg);
      if(section>=0){
	show_section(section,ok);
      } else {
	wprintf(L"wrong number, must in range 0x00 ~ 0xff\n");
      }
      return 0;
    case 't':
      decode_and_print(optarg);
      return 0;
    case 'g':
      symbol();
      return 0;
    case 'e':
      section=hexstr2i(optarg);
      if(section >=0){
	ext(section);
      } else {
	wprintf(L"wrong number, must in range 0x00 ~ 0xff\n");
      }
      return 0;
    }
  }

  for(int i=0;i<256;i++){
    show_section(i,ok);
  }
}
/*
0c ䷀ ䷁ ䷂ ䷃ ䷄ ䷅ ䷆ ䷇ ䷈ ䷉ ䷊ ䷋ ䷌ ䷍ ䷎ ䷏ 
0d ䷐ ䷑ ䷒ ䷓ ䷔ ䷕ ䷖ ䷗ ䷘ ䷙ ䷚ ䷛ ䷜ ䷝ ䷞ ䷟ 
0e ䷠ ䷡ ䷢ ䷣ ䷤ ䷥ ䷦ ䷧ ䷨ ䷩ ䷪ ䷫ ䷬ ䷭ ䷮ ䷯ 
0f ䷰ ䷱ ䷲ ䷳ ䷴ ䷵ ䷶ ䷷ ䷸ ䷹ ䷺ ䷻ ䷼ ䷽ ䷾ ䷿ 
 */
