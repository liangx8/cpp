/* -*-coding:utf-8-*- */
#include <windows.h>
#include <libintl.h>
#include <string>
#include <algorithm>

void errorMsg(DWORD errCode){
  LPVOID lpMsgBuf;

  FormatMessage(
				FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
				NULL,
				errCode,
				MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				(LPTSTR) &lpMsgBuf,
				0,
				NULL
				);
  printf("%s:[%d] %s\n",gettext("error"),errCode,lpMsgBuf);
  LocalFree( lpMsgBuf );

}


void walkDir(const char *top,void (*p)(const char *,LPWIN32_FIND_DATA)) {
  WIN32_FIND_DATA dir;
  char topName[1024];
  int toplen=strlen(top);
  //	WIN32_FIND_DATA sdir;
  strcpy(topName,top);
  topName[toplen]='/';
  topName[toplen+1]='*';
  topName[toplen+2]='\0';
  HANDLE h=FindFirstFile(topName,&dir);
  if(h==INVALID_HANDLE_VALUE){
	printf("%d>%s\n",__LINE__,topName);

	errorMsg(GetLastError());
	return;
  }
  while(1){
	topName[toplen]='/';
	topName[toplen+1]='\0';
	strcat(topName,dir.cFileName);
	if(dir.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
	  if(strlen(dir.cFileName)==1 && dir.cFileName[0]=='.') goto next_file;
	  if(strlen(dir.cFileName)==2 && dir.cFileName[0]=='.' && dir.cFileName[1]=='.') goto next_file;
	  walkDir(topName,p);
	} else {
	  p(topName,&dir);
	}
  next_file:
	if(!FindNextFile(h,&dir)){
	  int errCode=GetLastError();
	  if(errCode==ERROR_NO_MORE_FILES){
		break;
	  }
	  printf("%d>%s\n",__LINE__,top);
	  errorMsg(errCode);
	  break;
	}
  }
  if(!FindClose(h)){
	  printf("%d>%s\n",__LINE__,top);
	errorMsg(GetLastError());
  }
}
void walk(const char *top,void (*p)(const char *,LPWIN32_FIND_DATA)) {
  WIN32_FIND_DATA dir;
  HANDLE h=FindFirstFile(top,&dir);
  if(h==INVALID_HANDLE_VALUE){
	  printf("%d>%s\n",__LINE__,top);
	errorMsg(GetLastError());
	return;
  }
  if(dir.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY){
	walkDir(top,p);
  } else {
	p(top,&dir);
  }
  if(!FindClose(h)){

	errorMsg(GetLastError());
  }


  // case S_IFREG:
  // 	p(top,&st);	break;
  // default:       printf(gettext("unknown?\n"));                break;
  // } 
}
