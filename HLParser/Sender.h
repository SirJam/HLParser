#pragma once
#pragma comment(lib, "ws2_32.lib")

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <WinSock2.h>
#include <windows.h>

using namespace std;

#define PORT 3000
#define SERVERADDR "127.0.0.1"

class Sender
{
private:
	SOCKET *m_sock;
public:
	Sender()
	{
		// Пример простого TCPклиента
		char buff[1024];
		printf("TCP DEMO CLIENT\n");
		// Шаг 1  инициализация библиотеки Winsock
		if (WSAStartup(0x202, (WSADATA *) &buff[0]))
		{
			printf("WSAStart error %d\n", WSAGetLastError());
			return;
		}
		// Шаг 2  создание сокета
		m_sock = new SOCKET(socket(AF_INET, SOCK_STREAM, 0));
			//socket(AF_INET, SOCK_STREAM, 0);
		if (!m_sock)
		{
			printf("Socket() error %d\n", WSAGetLastError());
			return;
		}
		// Шаг 3  установка соединения
		// заполнение структуры sockaddr_in – указание адреса и порта сервера
		sockaddr_in dest_addr;
		dest_addr.sin_family = AF_INET;
		dest_addr.sin_port = htons(PORT);
		HOSTENT *hst;
		// преобразование IP адреса из символьного в сетевой формат
		if (inet_addr(SERVERADDR) != INADDR_NONE)
			dest_addr.sin_addr.s_addr = inet_addr(SERVERADDR);
		else
		{
			printf("Invalid address %s\n", SERVERADDR);
			closesocket(*m_sock);
			WSACleanup();
			return;
		}
		// адрес сервера получен – пытаемся установить соединение
		if (connect(*m_sock, (sockaddr *) &dest_addr, sizeof(dest_addr)))
		{
			printf("Connect error %d\n", WSAGetLastError());
			return;
		}
	}
	~Sender()
	{
		closesocket(*m_sock);
		WSACleanup();
	}

	struct Message
	{
		int line;
		string message;
	};

	void SendMessageWithDescription(int line, string message, bool isError)
	{
		// Шаг 4  чтение и передача сообщений
		char buff[1024];		
		ostringstream str;
		if (isError) {
			str << "Error at line: " << line << endl;
		}
		str << message << endl;

		for (int i = 0; i < str.str().size(); i++) {
			buff[i] = str.str().at(i);
		}
		// передаем строку клиента серверу
		send(*m_sock, &buff[0], sizeof(char) * str.str().size(), 0);
		if (isError) {
			exit(EXIT_FAILURE);
		}
	}
};