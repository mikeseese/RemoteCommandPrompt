// client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <winsock2.h>
#include <includes.h>

#include <winsockfunctions.h>

#pragma comment(lib, "WS2_32.Lib")

//Variables
char strPathArray[256];
char strCommandArray[256];
char strOutputArray[256];
char buf[256];
char done;
int Start;
int portNumber = 9999;
string strCommand;
string strPath;
string strAppPath;
string strOutput;
string strChangePath;
string strInput;
string ipAddress = "127.0.0.1";
SOCKET kSock = NULL;


int _tmain(int argc, _TCHAR* argv[])
{
	//Start winsock
	kSock = startSocket(kSock);
	done = 'f';
	if (kSock == 0)
	{
		system("pause");
		return 0;
	}
	socketListen(kSock, portNumber);
	kSock = accept(kSock, 0, 0);

	system("title CMD Emulator Client");
	while(strCommand != "exit")
	{
		emptyCharArray(buf);
		recv(kSock, buf, sizeof(buf), 0);
		strPath = buf;
		cout << strPath << ">";
		cin.getline(strCommandArray,256);
		strCommand = strCommandArray;
		if (strCommand == "exit") //if command is exit, send it and then close program
		{
			send(kSock, strCommandArray, sizeof(strCommandArray), 0);
			cout << "Exiting...";
			wait(1);
			return 0; 
		}
		else if (strCommand == "")
		{
			strCommandArray[0] = 'b';
			strCommandArray[1] = 'l';
			strCommandArray[2] = 'a';
			strCommandArray[3] = 'n';
			strCommandArray[4] = 'k';
			strCommandArray[5] = 'c';
			strCommandArray[6] = 'o';
			strCommandArray[7] = 'm';
			strCommandArray[8] = 'm';
			strCommandArray[9] = 'a';
			strCommandArray[10] = 'n';
			strCommandArray[11] = 'd';
		}
		send(kSock, strCommandArray, sizeof(strCommandArray), 0);
		emptyCharArray(buf);
		recv(kSock, buf, sizeof(buf), 0); //get and display first line
		cout << buf << endl;
		done = 'f';
		while(done != 't') //get and display 2nd line until eof
		{
			emptyCharArray(buf);
			recv(kSock, buf, sizeof(buf), 0);
			cout << buf << endl;
			emptyCharArray(buf);
			recv(kSock, buf, 1, 0);
			done = buf[0];
		}
	}
	return 0;
}