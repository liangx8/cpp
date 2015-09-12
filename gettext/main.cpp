// -*-coding:utf-8-*-

/**
 * xgettext -o main.pot main.cpp life_pattern.cpp
 * msginit -l zh_CN -i main.pot
 * msgfmt zh_CN.po -o locale/zh_CN/LC_MESSAGES/go.mo

 * LOCALEDIR = locale/
 * PACKAGE = go
 */


#include <clocale>
#include <cstdio>
#include <libintl.h>
#include <strings.h>
#include <time.h>
#define LOCALEDIR "locale/"
#define PACKAGE "sample"
#ifndef UNICODE
#define UNICODE
#endif


void showtime(struct tm *t){
  printf("%d-%d-%d %d:%d:%d %d\n",t->tm_year+1900,t->tm_mon+1,t->tm_mday,t->tm_hour,t->tm_min,t->tm_sec,timezone);
}
int main(int argc, char **argv){
  printf("current locale: %s\n",setlocale(LC_ALL,NULL));
  const char *loc_name=setlocale(LC_ALL,"");
  bindtextdomain(PACKAGE,LOCALEDIR);
  textdomain(PACKAGE);
  const char *dst=gettext("hello world!");
  printf("[%s]\n",dst);
  printf("%x,%x\n",dst[0],dst[1]);
  if(NULL != loc_name){
	printf("locale setting successful: %s\n",loc_name);
  } else {
	printf("locale setting failure!\n");
  }
  printf("current locale: %s\n",setlocale(LC_ALL,NULL));
  tzset();
  time_t t;
  t=time(NULL);
  struct tm *mt;
  mt=gmtime(&t);
  showtime(mt);
  mt=localtime(&t);
  showtime(mt);
  return 0;
}
