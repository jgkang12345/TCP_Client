
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

	// 1. 윈속 초기화 
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)	printf("Failed WSAStartup() \n");

	// 2. socket 생성
	hSocket = SocketHelper::CreateIP4Socket();
	if (hSocket == INVALID_SOCKET) printf("Failed socket() \n");

	// 3. 서버 연결
	//     3-1. 연결할 서버 주소 
	SocketHelper::InitIP4Socket(servAddr, "127.0.0.1", 3000);

	//     3-2 서버에 연결 시도
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
			printf("게임 종료!! 서버 승리\n");
			break;
		}
		else if (*recvData == 'q')
		{
			printf("게임 종료!! 클라 승리\n");
			break;
		}




		printf("1.가위 2.바위 3.보 선택해주세요! (숫자 1: 가위, 숫자 2: 바위, 숫자 3: 보)\n");
		{
			unsigned __int64 Timer = GetTickCount64();
			while (true)
			{
				if (GetTickCount64() >= Timer + 1000) //1초가 지났는가?
				{
					/*printf("1초 지남");*/
					Timer = GetTickCount64();
					count++;

					if (count == 5)
					{
						printf("5초 남았습니다...\n");
					}
					else if (count == 6)
					{
						printf("4초 남았습니다...\n");
					}
					else if (count == 7)
					{
						printf("3초 남았습니다...\n");
					}
					else if (count == 8)
					{
						printf("2초 남았습니다...\n");
					}
					else if (count == 9)
					{
						printf("1초 남았습니다...\n");
					}
					else if (count == 10)
					{
						printf("시간초과... 자동으로 선택됩니다.");
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
					printf("다른플레이어를 기다립니다...");
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
				printf("결과를 계산합니다...\n");
			}
		}

		{
			char recvData[256];
	

			ConsoleHelper::Clear();
			printf("결과를 계산합니다...\n");
			recv(hSocket, recvData, sizeof(recvData), 0);
			ConsoleHelper::Clear();
			int flags[20];
			split(flags, recvData, '#');

			switch (flags[0])
			{
			case RockPaperScissors::Rock:
				printf("서버 바위 \t");
				break;

			case RockPaperScissors::Scissors:
				printf("서버 가위 \t");
				break;

			case RockPaperScissors::Paper:
				printf("서버 보자기 \t");
				break;
			}

			switch (flags[1])
			{
			case RockPaperScissors::Rock:
				printf("클라 바위 \n");
				break;

			case RockPaperScissors::Scissors:
				printf("클라 가위 \n");
				break;

			case RockPaperScissors::Paper:
				printf("클라 보자기 \n");
				break;
			}

			switch (flags[2])
			{
			case P1:
				printf("서버 승리~!\n");
				break;

			case P2:
				printf("클라 승리~!\n");
				break;

			case DD:
				printf("무승부!\n");
				break;
			}

			Sleep(1500);

			ConsoleHelper::Clear();

		}
	}
	// 5. 소켓 종료 -> 윈속 종료
	closesocket(hSocket);
	WSACleanup();

	system("pause");
	return 0;
}