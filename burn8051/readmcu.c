#include <stdio.h>
#include <malloc.h>

#include <windows.h>
#include "dll.h"
HRESULT connectC2(const char *sn,int powertarget,int disableDialog);
HRESULT disconnectC2(void);
HRESULT run_target(void){
	HRESULT result=SetTargetGo();
	if(SUCCEEDED(result)){
		Sleep(1000);		// sleep 2 seconds
	} else {
		wprintf(L"error (%x)\n",result);
	}
	return result;
}
void writebin(char *file,BYTE *b,int size){
	FILE *fp=fopen(file,"wb");
	if(fp==NULL){
		wprintf(L"%s can't be created\n",file);
		return;
	}
	fwrite(b,1,size,fp);
	fclose(fp);
}
void readCodeMemory(char *file,DWORD size){
	BYTE *buf;
	HRESULT result;
	const char *pChar;
	result=GetUSBDeviceSN(0,&pChar);
	if(FAILED(result)){
		wprintf(L"Get device serial number error(%x)!\n",result);
		return;
	}
	result=connectC2(pChar,0,1);
	if(FAILED(result)){
		return;
	}
	
	buf=malloc(size);
	if(buf==NULL){
		wprintf(L"insufficient memory!\n");
		return;
	}
	result=GetCodeMemory(buf,0,size);
	if(FAILED(result)){
		wprintf(L"read code memory error\n");
	}
	disconnectC2();
	writebin(file,buf,size);
	free(buf);
	return ;
}
void show(BYTE *buf,int size){
	int i,j;
	wprintf(L"    ");
	for(i=0;i<16;i++){
		wprintf(L" %02x",i);
	}
	wprintf(L"\n");
	for (i=0;i<size;i++){
		wprintf(L"%04x",i*16);
		for(j=0;j<16;j++){
			wprintf(L" %02x",buf[i*16+j]);
		}
		wprintf(L"\t");
		for(j=0;j<16;j++){
			BYTE c=buf[i*16+j];
			if ((c>='0' && c <='9') || (c >='a' && c <='z') || (c >= 'A' && c <= 'Z'))
				wprintf(L"%c",c);
			else
				wprintf(L".");
		}
		wprintf(L"\n");
	}

}
void readMemory(void){
	BYTE buf[256];
	HRESULT result;
	const char *pChar;
	result=GetUSBDeviceSN(0,&pChar);
	if(FAILED(result)){
		wprintf(L"Get device serial number error(%x)!\n",result);
		return;
	}
	result=connectC2(pChar,0,1);
	if(FAILED(result)){
		return;
	}
	wprintf(L"Target running...\n");
	if(FAILED(run_target())){
		goto error_end;
	}

	result=SetTargetHalt();
	if(FAILED(result)){
		wprintf(L"Set target halt error(%x)\n",result);
		goto error_end;
		
	}
	result=GetRAMMemory(buf, 0, 256);
	if(FAILED(result)){
		wprintf(L"Read RAM error(%x)\n",result);
		goto error_end;
	}
	result=SetTargetGo();
	if(FAILED(result)){
		wprintf(L"Failed to run target MCU(%x)\n",result);
	}else {
		wprintf(L"Target running...\n");
	}

	show(buf,16);
error_end:
	disconnectC2();
}
// buffer size should be 1,2,3,4
void readXMemory(void){
	BYTE buf[512];
	HRESULT result;
	const char *pChar;
	result=GetUSBDeviceSN(0,&pChar);
	if(FAILED(result)){
		wprintf(L"Get device serial number error(%x)!\n",result);
		return;
	}
	result=connectC2(pChar,0,1);
	if(FAILED(result)){
		return;
	}
	wprintf(L"Target running...\n");
	if(FAILED(run_target())){
		goto error_end;
	}
	result=SetTargetHalt();
	if(FAILED(result)){
		wprintf(L"Set target halt error(%x)\n",result);
		goto error_end;
		
	}
	result=GetXRAMMemory(buf, 0, 512);
	if(FAILED(result)){
		wprintf(L"Read XRAM error(%x)\n",result);
		goto error_end;
	}
	show(buf,32);

	Sleep(1000);

	result=SetTargetGo();
	if(FAILED(result)){
		wprintf(L"Failed to run target MCU(%x)\n",result);
	}

error_end:
	disconnectC2();
}

void interAction(void){
	BYTE buf[512];
	HRESULT result;
	const char *pChar;
	int	is_running;
	result=GetUSBDeviceSN(0,&pChar);
	if(FAILED(result)){
		wprintf(L"Get device serial number error(%x)!\n",result);
		return;
	}
	result=connectC2(pChar,0,1);
	if(FAILED(result)){
		return;
	}
	wprintf(L"Target running...\n");
	if(FAILED(run_target())){
		goto error_end;
	}
	is_running=1;
	wprintf(
L"***************************************\n\
*              debug mode             *\n\
***************************************\n\
\tq - quit\n\
\tx - read XRAM\n\
\ti - read RAM\n\
\th - halt target\n\
\tg - run target\n"
);

	while(1){
		char c;

		c=getchar();
		switch(c){
			case 'q':goto error_end;
			case '\n':
				wprintf(L"[r/q/x/i/h/g]\n");
				if(is_running)
					wprintf(L"R:");
				else
					wprintf(L"H:");
				break;
			case 'h':
				if(!is_running) break;
				result=SetTargetHalt();
				if(FAILED(result)){
					wprintf(L"Set target halt error(%x)\n",result);
					goto error_end;
					
				}
				wprintf(L"Target halt\n");
				is_running=0;
				break;
			case 'g':
				if(is_running) break;
				if(FAILED(run_target())){
					goto error_end;
				}
				wprintf(L"target running\n");
				is_running=1;
				break;
			case 'x':
				if(is_running){
					wprintf(L"halt target first\n");
				}else {
					
					result=GetXRAMMemory(buf, 0, 512);
					if(FAILED(result)){
						wprintf(L"Read XRAM error(%x)\n",result);
						goto error_end;
					}
					show(buf,32);
				}
				break;
			case 'i':
				if(is_running){
					wprintf(L"halt target first\n");
				}else {
					result=GetRAMMemory(buf, 0, 256);
					if(FAILED(result)){
						wprintf(L"Read RAM error(%x)\n",result);
						goto error_end;
					}
					show(buf,16);
				}
				break;

			default:
				break;
		}
		
	}

error_end:
	disconnectC2();
}
