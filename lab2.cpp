// lab2.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>
#include <pdh.h>
#include "atlconv.h"
#include <iomanip>

void PrintMemoryInfo() {
	MEMORYSTATUSEX statusex;

	statusex.dwLength = sizeof(statusex);
	GlobalMemoryStatusEx(&statusex);
	std::cout << "RAM: " << (statusex.ullTotalPhys - statusex.ullAvailPhys) / 1024 / 1024 << "MB / " <<
		statusex.ullTotalPhys / 1024 / 1024 << "MB " << statusex.dwMemoryLoad << "% " << std::endl;
	std::cout << "Pagefile: " << (statusex.ullTotalPageFile - statusex.ullAvailPageFile) / 1024 / 1024 << "MB / " <<
		statusex.ullTotalPageFile / 1024 / 1024 << "MB " << ((float)statusex.ullTotalPageFile - statusex.ullAvailPageFile) / statusex.ullTotalPageFile * 100 << "% " << std::endl;
}
void printHeader() {
	std::cout << std::setw(40) << "ProcessName" << " " << std::setw(5) <<
		"PID" << " " << std::setw(13) <<
		"PeakMem" << std::setw(13) <<
		"CurrentMem:" << std::setw(13) <<
		"PeakPagefile" << std::setw(13) <<
		"PageFile" << std::endl;

}
void printProcInfo(DWORD pid) {
	HANDLE hprocess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, false, pid);
	if (hprocess == NULL)
		return;
	PROCESS_MEMORY_COUNTERS pmc;
	if (!GetProcessMemoryInfo(hprocess, &pmc, sizeof(pmc)))
		pmc.PeakWorkingSetSize = pmc.WorkingSetSize = pmc.PagefileUsage = pmc.PeakPagefileUsage = 0;
	HMODULE hMod;
	DWORD cbNeeded;
	TCHAR processName[MAX_PATH] = TEXT("?");
	EnumProcessModules(hprocess, &hMod, sizeof(hMod), &cbNeeded);
	GetModuleBaseName(hprocess, hMod, processName, sizeof(processName) / sizeof(TCHAR));

	std::wcout << std::setw(40) << processName << " " << std::setw(5) <<
		pid << " " << std::setw(10) <<
		pmc.PeakWorkingSetSize / 1024 / 1024 << std::setw(3) << " MB" << std::setw(10) <<
		pmc.WorkingSetSize / 1024 / 1024 << std::setw(3) << " MB " << std::setw(10) <<
		pmc.PeakPagefileUsage / 1024 / 1024 << std::setw(3) << " MB " << std::setw(10) <<
		pmc.PagefileUsage / 1024 / 1024 << std::setw(3) << " MB " <<  std::endl;
}
int main()
{
	int timer;
	std::cin >> timer;
	do
	{
		PrintMemoryInfo();
		DWORD pids[1024], processes_bytes_size, num_processes;
		unsigned int i;
		EnumProcesses(pids, sizeof(pids), &processes_bytes_size);
		num_processes = processes_bytes_size / (sizeof(DWORD));
		printHeader();
		for (unsigned int i = 0; i < num_processes; i++)
			printProcInfo(pids[i]);
		Sleep(timer * 1000);
	} while (timer);
}