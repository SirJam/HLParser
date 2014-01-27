#pragma once

#include <sstream>
#include <iostream>
#include <windows.h>
#include <stdlib.h> 
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/types.h>
#include <socketapi.h>

using namespace std;

class Sender
{
private:
	LPCWSTR const pipeName()
	{
		return L"\\\\.\\pipe\\god_c_creator_pipe";
	}
	HANDLE hPipe;

public:
	Sender()
	{
	}

	struct Message
	{
		int line;
		string message;
	};
	
	void SendMessageWithDescription(int line, string message)
	{
		int sockfd;

		struct addrinfo *servinfo, *p;
		int rv;
		int numbytes;
		char* hostname = "localhost";
		char* server = "localhost";
	/*	Message msg;
		msg.line = line;
		msg.message = message;

		hPipe = CreateFile(pipeName(), GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		DWORD bytes = 0;
		if (hPipe == INVALID_HANDLE_VALUE) {
			cout << "There is no pipe channel!" << endl;
		}

		WriteFile(hPipe, (LPVOID) &message, sizeof(message), &bytes, NULL);


		// Flush the buffer
		FlushFileBuffers(hPipe);*/
	}
};