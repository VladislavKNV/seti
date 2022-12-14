#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "stdafx.h"
#include <string>
#include <iostream>
#include "Winsock2.h" //заголовок WS2_32.dll
#pragma comment(lib, "WS2_32.lib") //экспорт WS2_32.dll
#include "Error.h"
#include <ctime>

using namespace std;

SOCKET sS;


bool GetRequestFromClient(char *name, short port, sockaddr* from, int* flen)
{
	char nameServer[50];
	memset(from, 0, sizeof(flen));

	if ((sS = socket(AF_INET, SOCK_DGRAM, NULL)) == INVALID_SOCKET)
	{
		throw  SetErrorMsgText("socket:", WSAGetLastError());
	}

	SOCKADDR_IN serv;
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	serv.sin_addr.s_addr = INADDR_ANY;

	if (bind(sS, (LPSOCKADDR)&serv, sizeof(serv)) == SOCKET_ERROR)
	{
		cout << endl << "Server Port: " << serv.sin_port;
		cout << endl << "Server IP: " << inet_ntoa(serv.sin_addr);
		throw  SetErrorMsgText("bind:", WSAGetLastError());
	}

	if ((recvfrom(sS, nameServer, sizeof(nameServer), NULL, from, flen)) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAETIMEDOUT)
		{
			return false;
		}
		else
		{
			throw  SetErrorMsgText("recv:", WSAGetLastError());
		}
	}
	else cout << endl << "Message: " << nameServer << endl;


	if (strcmp(nameServer, name)!=0)
	{
		cout << endl << "WRONG SERVER NAME."<<endl;
		return true;
	}
	else
	{
		cout << endl << "TRUE SERVER NAME" << endl;
	}
}

void PutAnswerToClient(char* name, sockaddr* to, int* lto)
{
	if ((sendto(sS, name, strlen(name) + 1, NULL, to, *lto)) == SOCKET_ERROR)
	{
		throw  SetErrorMsgText("send:", WSAGetLastError());
	}
}

int main()
{
	setlocale(LC_CTYPE, "rus");

	WSADATA wsaData;

	try
	{
		if (WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
		{
			throw  SetErrorMsgText("Startup:", WSAGetLastError());
		}

		char name[] = "Vlad";

		for (;;)
		{
			SOCKADDR_IN clnt;
			int lc = sizeof(clnt);

			if (GetRequestFromClient(name, 2000, (sockaddr*)&clnt, &lc))
			{
				PutAnswerToClient(name, (sockaddr*)&clnt, &lc);
			}
			else
			{
			}

			SOCKADDR_IN* addr = (SOCKADDR_IN*)&clnt;
			cout << endl << "Client Port: " << addr->sin_port;
			cout << endl << "Client IP: " << inet_ntoa(addr->sin_addr);

			if (closesocket(sS) == SOCKET_ERROR)
			{
				throw  SetErrorMsgText("closesocket:", WSAGetLastError());
			}
		}

		if (WSACleanup() == SOCKET_ERROR)
		{
			throw  SetErrorMsgText("Cleanup:", WSAGetLastError());
		}
	}
	catch (string errorMsgText)
	{
		cout << endl << errorMsgText;
	}

	cout << endl;
	system("pause");
	return 0;
}