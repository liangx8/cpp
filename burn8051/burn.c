/*
2013-6-3
快速烧写8051芯片
-d 不显示对话框，对于被保护的芯片，如果不用-e先删除的话，会
   导致烧写失败
-e 烧写前先删除flash
-l 保护程序不被读取

2013-7-17
refer to an117.pdf
compiling depend on siutil.lib
runing depend on SiUtl.dll and USBHID.dll,
*/

#include <stdio.h>
#include <sys/stat.h>
#include <getopt.h>
#include <locale.h>
#include "burn.h"
#include "dll.h"
void readCodeMemory(char *file,DWORD size);
void readXMemory(void);
void readMemory(void);


HRESULT connectC2(const char *serialName,int powerTarget,int disableDialogBox){
	HRESULT result;
	result=ConnectUSB(serialName,C2_PROTOCOL,powerTarget,disableDialogBox);
	if(FAILED(result)){
		wprintf(L"connect failure!(%x)\n",result);
	} else {
		wprintf(L"connect established!\n");
	}
	return result;
}
HRESULT disconnectC2(void){
	HRESULT result;
	result=DisconnectUSB();
	if(FAILED(result)){
		wprintf(L"disconnect isn't completed!(%x)\n",result);
	}
	return result;
}
HRESULT downloadC2(const char *hexFile,int codeErase,int disableDialog, int flag,int bank,int lock,int persistFlash){
	wprintf(L"正在烧写 ...\n");
	return Download((char*)hexFile,codeErase,disableDialog,flag,bank,lock,persistFlash);
}
wchar_t *ascii2unicode(const char *src){
	int c=strlen(src);
	int i;
	wchar_t *wc=malloc(sizeof(wchar_t)*(c+1));
	for(i=0;i<c;i++){
		wc[i]=src[i];
	}
	wc[c]=0;
	return wc;
}
void listUSB(int count,const char *hex,struct data *d){
	DWORD i;
	wchar_t *wc;
	HRESULT result;
	for(i=0;i<count;i++){
		const char *pChar;
		result=GetUSBDeviceSN(i, &pChar);
		if(SUCCEEDED(result)){
			if(i==0){
				if(SUCCEEDED(connectC2(pChar,0,1))){
					result=SetTargetHalt();
					if(FAILED(result)){
						wprintf(L"Failed to halt target MCU(%x)\n",result);
					} else {
						result=downloadC2(d->hex,d->erase,d->disableDialog,0,-1,d->lock,1);
						if(FAILED(result)){
							wprintf(L"烧写出错(%X)\n",result);
						} else {
							wprintf(L"*********************************************\n");
							wprintf(L"*             烧写成功                      *\n");
							wprintf(L"*********************************************\n");
						}
						if(d->goRun){
							result=SetTargetGo();
							if(FAILED(result)){
								wprintf(L"Failed to run target MCU(%x)\n",result);
							}else {
								wprintf(L"Target running...\n");
							}
							
						}
					}
				}
				disconnectC2();
			}
			wc=ascii2unicode(pChar);
			wprintf(L"%d:%s\n",(int)i,wc);
			free(wc);
		} else {
			wprintf(L"Get device serial number error!\n");
		}
	}
}
void usage(void){
	wprintf(
L"Usage:\nBurn hex framework to c8051. Copyright by Holy-win, Version 0.1\n\
burn [option] <target>\n\
\t-l	Don't lock user space\n\
\t-e	Don't erase before download\n\
\t-d	don't show dialog\n\
\t-R	run target after burn\n\
\ttarget	hex file\n\n\
\t-r n	read code memory\n\
\t\t n = 1, 8K\n\
\t\t n = 2, 16K\n\
\t\t n = 3, 32K\n\
\ttarget	output file\n\n\
\t-m	show xram\n\
\ttarget is omited\n"
);
}

int main(int argc,char **argv){
	DWORD dwDevices;
	HRESULT result;
	struct data d;
	int opt;
	struct stat buf;
	const char *dllUSBVersionStr;
	char *dllVersionStr;
	wchar_t *wbuf;
	
	int readXram=0;
	int readRam=0;

	DWORD readSize;
	int readMCU=0;
	
	setlocale(LC_ALL,"");
	
	
	result=GetUSBDLLVersion(&dllUSBVersionStr);

	if(FAILED(result)){
		wprintf(L"dll version error\n");
	} else {
		wbuf=ascii2unicode(dllUSBVersionStr);
		wprintf(L"USB DLL version:%s\n",wbuf);
		free(wbuf);
	}
	dllVersionStr=GetDLLVersion();
	if(dllVersionStr){
		wbuf=ascii2unicode(dllVersionStr);
		wprintf(L"DLL version:%s\n",wbuf);
		free(wbuf);
	}
	
	d.erase=1;
	d.lock=1;
	d.disableDialog=0;
	d.goRun=0;
	if(argc == 1) {
		usage();
		return 0;
	}
	while((opt = getopt(argc,argv,"ledr:Rm:")) != -1){
		switch(opt){
			case 'l':d.lock=0;break;
			case 'e':d.erase=0;break;
			case 'd':d.disableDialog=1;break;
			case 'R':d.goRun=1;break;
			case 'm':
				switch(optarg[0]){
					case 'x':readXram=1;break;
					case 'i':readRam=1;break;
					default:
						wprintf(L"-m should be 'x' or 'i'\n");
						usage();return 0;
				}

			break;
			case 'r':

				readMCU=1;
				switch(atoi(optarg)){
					case 1:readSize=8*1024;break;
					case 2:readSize=16*1024;break;
					case 3:readSize=32*1024;break;
					default:
						wprintf(L"-r should 1,2,3\n");
						usage();return 0;
				}
				break;
			
			case '?':
			default:
				usage();return 0;
		}
	}
	result=USBDebugDevices(&dwDevices);
	if(SUCCEEDED(result)){
		if(dwDevices <=0){
			wprintf(L"No USB device found!\n");
			return 0;
		}
		if(readXram){
			readXMemory();
			return 0;
		}
		if(readRam){
			readMemory();
			return 0;
		}

		if(optind >= argc){
			wprintf(L"target file required!\n");
			usage();
			return 0;
		}

		d.hex=argv[optind];
		if(!readMCU){
			if(stat(d.hex,&buf)){

				wbuf=ascii2unicode(d.hex);
				wprintf(L"file %s open error\n",wbuf);
				free(wbuf);

				return 0;
			}
			if(!S_ISREG(buf.st_mode)){
				wbuf=ascii2unicode(d.hex);
				wprintf(L"%s is not a regular file\n",wbuf);
				free(wbuf);
				return 0;
			}
		}


		if(readMCU){
			
			readCodeMemory(argv[optind],readSize);
			return 0;
		}

		wprintf(L"找到%d个烧写设备\n",(int)dwDevices);
		while(1){
			char c;
			listUSB(dwDevices,argv[1],&d);
			wprintf(L"继续吗(Y/n)?");
			c=getchar();
			if(c != '\n')break;
		}
	} else {
		wprintf(L"search device error\n");
	}
	
	return 0;
}
