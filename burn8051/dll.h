#ifndef _DLL_H_
#define _DLL_H_
#include <windows.h>
//extern "C" __declspec(dllimport) 
HRESULT __stdcall Connect(int nComPort, int nDisableDialogBoxes, int nECprotocol, int nBaudRateIndex);
//extern "C" __declspec(dllimport) 
HRESULT __stdcall ConnectUSB(const char* sSerialNum, int nECprotocol, int nPowerTarget, int nDisableDialogBoxes);
//extern "C" __declspec(dllimport) 
HRESULT __stdcall GetUSBDLLVersion(const char **pVersionString);
//extern "C" __declspec(dllimport) 
char* __stdcall GetDLLVersion();
//extern "C"
__declspec(dllimport) HRESULT __stdcall Download(char* sDownloadFile, 
														int nDeviceErase, 
														int nDisableDialogBoxes, 
														int nDownloadScratchPadSFLE, 
														int nBankSelect, 
														int nLockFlash, 
														BOOL bPersistFlash);
//extern "C" __declspec(dllimport) 
HRESULT __stdcall Disconnect(int nComPort);
//extern "C" __declspec(dllimport) 
HRESULT __stdcall DisconnectUSB();
// dwDevices usb的数量
//extern "C" 
__declspec(dllimport) HRESULT __stdcall USBDebugDevices(DWORD *dwDevices);
// 获取每个usb的名字
//extern "C" __declspec(dllimport) 
HRESULT __stdcall GetUSBDeviceSN(DWORD dwDeviceNum, const char **psSerialNum);

//extern "C" 
__declspec(dllimport) HRESULT __stdcall GetRAMMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength);
//extern "C" 
__declspec(dllimport) HRESULT __stdcall SetRAMMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength);
//extern "C" 
__declspec(dllimport) HRESULT __stdcall GetXRAMMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength);
//extern "C" 
__declspec(dllimport) HRESULT __stdcall SetXRAMMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength);
//extern "C" 
__declspec(dllimport) HRESULT __stdcall GetCodeMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength);
//extern "C" 
__declspec(dllimport) HRESULT __stdcall SetCodeMemory(BYTE * ptrMem, DWORD wStartAddress, unsigned int nLength,
												   BOOL bDisableDialogs);
__declspec(dllimport) HRESULT __stdcall SetTargetGo();
__declspec(dllimport) HRESULT __stdcall SetTargetHalt();


#ifndef FAILED
#define FAILED(status)	((HRESULT)(status) < 0)
#define SUCCEEDED(status)	((HRESULT)(status) >= 0)
#endif
#endif
