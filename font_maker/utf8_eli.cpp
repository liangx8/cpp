#include <windows.h>
#include <iostream>
void eliminate_utf8(HANDLE src_file){
	unsigned char read_buf[1024];
	DWORD	bytes_to_read;
	int offset=0;
	int k=0;
	OVERLAPPED ol;
	ol.Offset =0;
	ol.OffsetHigh=0;
	ol.hEvent =NULL;

	while(ReadFile(src_file,&read_buf,1024,&bytes_to_read,&ol)){
		
		for(int i=offset;i<(int)bytes_to_read;i++){
			if(read_buf[i]>127){
				std::cout << ' ';
			} else {
				if(read_buf[i] == '\r') {
					continue;
				}
				std::cout << read_buf[i];
			}
			
		}
		ol.Offset=bytes_to_read+k*1024;
		k++;
	}
	
	
}

void showError(DWORD errCode){
		std::cout << "error code:" << errCode << std::endl;
		LPVOID lpMsgBuf;
		LPCTSTR msg;
		msg=(LPCTSTR)LocalAlloc(LPTR,1024);
		if(!msg){
			std::cout<< "error again" << std::endl;
		}
		
 
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			errCode,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
		);
		std::cout <<(char *)msg<<"Message:"<<(char*)lpMsgBuf<< std::endl;

		// Display the string.
		MessageBox( NULL, (char *)msg, "Error Code", MB_OK|MB_ICONINFORMATION );
		std::cout << "message:"<<(char *)lpMsgBuf << std::endl;

		// Free the buffer.
		LocalFree((LPVOID)msg);
		LocalFree( lpMsgBuf );

}

HANDLE openFile(LPCTSTR lpFileName,DWORD dwDesiredAccess){
	HANDLE f = CreateFile(
		lpFileName,
		dwDesiredAccess,
		FILE_SHARE_READ,
		NULL,
		//OPEN_ALWAYS,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(f==INVALID_HANDLE_VALUE){
		std::cout<<"Open file error"<<std::endl;
		
		showError(GetLastError());
		return NULL;
	}
	return f;
}
HANDLE createFile(LPCTSTR lpFileName,DWORD dwDesiredAccess){
	HANDLE f = CreateFile(
		lpFileName,
		dwDesiredAccess,
		FILE_SHARE_READ,
		NULL,
		//OPEN_ALWAYS,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if(f==INVALID_HANDLE_VALUE){
		std::cout<<"Open file error"<<std::endl;
		
		showError(GetLastError());
		return NULL;
	}
	return f;
}
int main(int argc,char **argv){
	if(argc == 2){
		HANDLE src_file;
		if((src_file=openFile(argv[1],GENERIC_READ))!=NULL){
			//dst_file=createFile(argv[2],GENERIC_WRITE);
			eliminate_utf8(src_file);
			CloseHandle(src_file);
			//CloseHandle(dst_file);
			
		}
		
	} else {
		std::cout << "usage: \n\t" << argv[0] << " <src> <dst>\n" << std::endl;
	}
	return 0;
}