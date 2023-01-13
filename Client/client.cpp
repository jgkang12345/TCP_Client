
#include <winsock2.h>
#include <WS2tcpip.h>
#include <conio.h>
#include <stdio.h>
#include "ConsoleHelper.h"
#include "SocketHelper.h"
#pragma comment( lib, "ws2_32.lib")

void split(int* arr, char* str, char sp)
{
	int index = 0;
	int size = strlen(str);
	for (int i = 0; i < size; i++)
	{
		arr[index] = (str[i]) - 48;
		if (sp == '#')
		{
			index++;
			i++;
		}
	}
}

enum RockPaperScissors : int
{
	Scissors = 1,
	Rock = 2,
	Paper = 3
};

enum Result : int
{
	P1 = 0,
	P2 = 1,
	DD = 2
};


int main(void)
{
	WSADATA  wsaData;
	SOCKET   hSocket;
	SOCKADDR_IN  servAddr;

	ConsoleHelper::ShowConsoleCursor(false);

	// 1. ���� �ʱ�ȭ 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	printf("Failed WSAStartup() \n");

	// 2. socket ����
	hSocket = SocketHelper::CreateIP4Socket();
	if (hSocket == INVALID_SOCKET) printf("Failed socket() \n");

	// 3. ���� ����
	//     3-1. ������ ���� �ּ� 
	SocketHelper::InitIP4Socket(servAddr, "127.0.0.1", 3000);

	//     3-2 ������ ���� �õ�
	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
	{
		printf("Failed connect() \n");
		return 1;
	}
	Sleep(1000);


	printf("connect ok....... \n");
	ConsoleHelper::Clear();

	while (true)
	{

		int  count = 0;
		char key = 0;

		char recvData[256];
		recv(hSocket, recvData, sizeof(recvData), 0);

		if (*recvData == 'p')
		{
			printf("���� ����!! ���� �¸�\n");
			break;
		}
		else if (*recvData == 'q')
		{
			printf("���� ����!! Ŭ�� �¸�\n");
			break;
		}




		printf("1.���� 2.���� 3.�� �������ּ���! (���� 1: ����, ���� 2: ����, ���� 3: ��)\n");
		{
			unsigned __int64 Timer = GetTickCount64();
			while (true)
			{
				if (GetTickCount64() >= Timer + 1000) //1�ʰ� �����°�?
				{
					/*printf("1�� ����");*/
					Timer = GetTickCount64();
					count++;

					if (count == 5)
					{
						printf("5�� ���ҽ��ϴ�...\n");
					}
					else if (count == 6)
					{
						printf("4�� ���ҽ��ϴ�...\n");
					}
					else if (count == 7)
					{
						printf("3�� ���ҽ��ϴ�...\n");
					}
					else if (count == 8)
					{
						printf("2�� ���ҽ��ϴ�...\n");
					}
					else if (count == 9)
					{
						printf("1�� ���ҽ��ϴ�...\n");
					}
					else if (count == 10)
					{
						printf("�ð��ʰ�... �ڵ����� ���õ˴ϴ�.");
						count = 0;
						key = ((rand() % 3) + 1) + 48;
						break;
					}
				}
				if (key == 0 && _kbhit())
				{
					key = _getch();
					if (key != '1' && key != '2' && key != '3')
						key = ((rand() % 3) + 1) + 48;
					printf("�ٸ��÷��̾ ��ٸ��ϴ�...");
					count = 0;
					break;
				}
			}
			char recvData[256];
			char sendBuffer[256];
			sendBuffer[0] = key;
			send(hSocket, sendBuffer, sizeof(sendBuffer), 0);
			recv(hSocket, recvData, sizeof(recvData), 0);
			if (*recvData == 't')
			{
				ConsoleHelper::Clear();
				printf("����� ����մϴ�...\n");
			}
		}

		{
			char recvData[256];
	

			ConsoleHelper::Clear();
			printf("����� ����մϴ�...\n");
			recv(hSocket, recvData, sizeof(recvData), 0);
			ConsoleHelper::Clear();
			int flags[20];
			split(flags, recvData, '#');

			switch (flags[0])
			{
			case RockPaperScissors::Rock:
				printf("���� ���� \t");
				break;

			case RockPaperScissors::Scissors:
				printf("���� ���� \t");
				break;

			case RockPaperScissors::Paper:
				printf("���� ���ڱ� \t");
				break;
			}

			switch (flags[1])
			{
			case RockPaperScissors::Rock:
				printf("Ŭ�� ���� \n");
				break;

			case RockPaperScissors::Scissors:
				printf("Ŭ�� ���� \n");
				break;

			case RockPaperScissors::Paper:
				printf("Ŭ�� ���ڱ� \n");
				break;
			}

			switch (flags[2])
			{
			case P1:
				printf("���� �¸�~!\n");
				break;

			case P2:
				printf("Ŭ�� �¸�~!\n");
				break;

			case DD:
				printf("���º�!\n");
				break;
			}

			Sleep(1500);

			ConsoleHelper::Clear();

		}
	}
	// 5. ���� ���� -> ���� ����
	closesocket(hSocket);
	WSACleanup();

	system("pause");
	return 0;
}