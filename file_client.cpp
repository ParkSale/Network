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
	SOCKET sock;
	FILE* fp;
	char buf[BUF_SIZE];
	int readCnt;

	SOCKADDR_IN servAddr;
	if (argc != 3) {
		printf("IP랑 PORT번호 입력하세요\n");
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}
	fp = fopen("receive.txt", "wb");
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		ErrorHandling("socket() error!");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));
	connect(sock, (SOCKADDR*)&servAddr, sizeof(servAddr));

	while ((readCnt = recv(sock, buf, BUF_SIZE, 0)) != 0) {
		fwrite((void*)buf, 1, readCnt, fp);
	}

	puts("Received file data");
	send(sock, "Thank you", 10, 0);
	fclose(fp);
	closesocket(sock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}