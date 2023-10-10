#pragma warning(disable : 4530)
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS

#define safe_read(Addr, Type) driver::read<Type>(g_Sock, g_PID, Addr)
#define safe_write(Addr, Data, Type) driver::write<Type>(g_Sock, g_PID, Addr, Data)
#define safe_memcpy(Dst, Src, Size) driver::write_memory(g_Sock, g_PID, Dst, driver::read_memory(g_Sock, g_PID, Src, 0, Size), Size)

#include <Windows.h>
#include <psapi.h>
#include <d3d11.h>
#include <iostream>
#include <string>
#include <cassert>
#include <emmintrin.h>
#include <tlhelp32.h>
#include <winternl.h>

#include <stdio.h>
#include <stdlib.h>
#include <float.h>

#include "driver.h"


SOCKET g_Sock;
DWORD g_PID;
uint64_t u_Base;
uint64_t g_Base;
bool g_Locked;

#include <thread>

#include <chrono>
#include <random>



DWORD GetPID(const std::wstring& procname);
DWORD RPid();


#define AllocCons 0
using namespace std;



long long firstentry = 0;

UINT64 TodCycle = 0;
bool panic = false;



#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <string>
#include <iostream>

#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iomanip>
#include <vector>



using namespace std;

#include <Wininet.h>
#pragma comment(lib, "wininet.lib")



#include <iostream>
#include <tlhelp32.h>
#include <psapi.h>


void main()
{

	{
		try
		{
			
			driver::initialize();
			cout << "OK" << endl;
			g_Locked = false;
			cout << "Connecting...";
			g_Sock = driver::connect();
			if (g_Sock == INVALID_SOCKET)
			{
				cout << "Connection fail!\n";
				cin.get();
				return;
			}
			cout << "Successfully" << endl;
		}
		catch (...) {
			cout << "Driver ERROR!" << endl;
			cin.get();
			return;
		}
		g_PID = RPid();
		cout << "pid: " << g_PID << endl;
		cout << "Getting base address...";
		u_Base = driver::get_process_base_address(g_Sock, g_PID, 0); //0 - UnityPlayer.dll, 
		g_Base = driver::get_process_base_address(g_Sock, g_PID, 1); //1 - GameAssembly.dll
		
		if (u_Base == 0)
		{
			cout << "u_Base not found\n";
			cin.get();
			return;
		}
		else
			if (g_Base == 0)
			{
				cout << "g_Base not found\n";
				cin.get();
				return;
			}
		
	}
	
}





DWORD RPid()
{
	if (g_PID) {
		if (!safe_read(g_Base, int)) {
			std::cout << "Rust closed...(or driver fail)" << std::endl;
			std::cin.get();
			exit(1);
		}
		return g_PID;
	}
	else {
		DWORD pid = GetPID(L"RustClient.exe");

		if (!pid) {
			std::cout << "Rust closed..." << std::endl;
			std::cin.get();
			exit(1);
		}
		return pid;
	}
}

DWORD GetPID(const std::wstring& procname)
{
	PROCESSENTRY32 processInfo;
	processInfo.dwSize = sizeof(processInfo);

	HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	if (processesSnapshot == INVALID_HANDLE_VALUE) {
		return 0;
	}

	Process32First(processesSnapshot, &processInfo);
	if (!procname.compare(processInfo.szExeFile))
	{
		CloseHandle(processesSnapshot);
		return processInfo.th32ProcessID;
	}

	while (Process32Next(processesSnapshot, &processInfo))
	{
		if (!procname.compare(processInfo.szExeFile))
		{
			CloseHandle(processesSnapshot);
			return processInfo.th32ProcessID;
		}
	}

	CloseHandle(processesSnapshot);
	return 0;
}




