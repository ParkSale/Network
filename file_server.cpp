#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#define BUF_SIZE 30
void ErrorHandling(char* message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET servSock, clntSock;
	FILE* fp;
	char buf[BUF_SIZE];
	int readCnt, clntAddrSz;;

	SOCKADDR_IN servAddr, clntAddr;
	if (argc != 2) {
		printf("port번호만 입력하세요\n");
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

	fp = fopen("hello.txt", "rb");
	servSock = socket(PF_INET, SOCK_STREAM, 0);
	if (servSock == INVALID_SOCKET) {
		ErrorHandling("socket creation error!");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		ErrorHandling("bind() error");
	}
	listen(servSock, 5);

	clntAddrSz = sizeof(clntAddr);
	clntSock = accept(servSock, (SOCKADDR*)&clntAddr, &clntAddrSz);

	while (1) {
		readCnt = fread((void*)buf, 1, BUF_SIZE, fp);
		if (readCnt < BUF_SIZE) {
			send(clntSock, (char*)&buf, readCnt, 0);
			break;
		}
		send(clntSock, (char*)&buf, BUF_SIZE, 0);
	}

	shutdown(clntSock, SD_SEND);
	recv(clntSock, (char*)buf, BUF_SIZE, 0);
	printf("Message from client : %s \n", buf);

	fclose(fp);
	closesocket(clntSock);
	closesocket(servSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}