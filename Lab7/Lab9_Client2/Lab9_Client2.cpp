#include "stdafx.h"
#include <windows.h>
#include <iostream>
#include "time.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	system("cls");
	HANDLE hslot;
	hslot = CreateFile(L"\\\\.\\mailslot\\demoSlot", GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, NULL, NULL);
	if (hslot == INVALID_HANDLE_VALUE)
	{

		std::cout << "WRITE FAIL" << std::endl;
		std::cout << "Press any key to exit" << std::endl;
		std::cin.get();
		return 0;

	}
	char wbuf[50] = "From Client2, Message ";
	int nData;
	int n = 10;
	DWORD dwBytesWrite;
	double t1, t2;

	t1 = clock();

	for (int i = 0; i<5; i++)
	{
		char q[6];
		sprintf_s(q, "%d", i + 1);
		char buf[sizeof(wbuf) + sizeof(q)];
		strcpy_s(buf, wbuf);
		strcat_s(buf, q);

		if (!WriteFile(hslot, buf, sizeof(wbuf), &dwBytesWrite, NULL))
		{
			std::cout << "WRITE FAIL" << std::endl;
			CloseHandle(hslot);
			std::cout << "Press any key to exit" << std::endl;
			std::cin.get();
		}

		cout << "Client2, Message " << i + 1 << endl;
	}

	t2 = clock();

	if (!WriteFile(hslot, " ", sizeof(" "), &dwBytesWrite, NULL))
	{
		std::cout << "WRITE FAIL" << std::endl;
		CloseHandle(hslot);
		std::cout << "Press any key to exit" << std::endl;
		std::cin.get();
	}

	if (!CloseHandle(hslot))
		throw "Error: CloseHandle";

	cout << endl << "Time: " << (t2 - t1) / 1000 << " sec." << endl << endl;
	std::cout << "Press any key to exit" << std::endl;
	CloseHandle(hslot);
	std::cin.get();
	return 0;


	return 0;
}

