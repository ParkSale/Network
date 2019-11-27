#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
void Error_handling(char* message);

int main(int argc, char* argv[]) {
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	int szClntAddr;
	char message[] = "Hello World!";
	printf("%d\n", argc);
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	//WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData) : (������ ��������,����ü ������ �ּ� �� ����)
	//MAKEWORD(1,2) : �ֹ���1 �ι���2 (1.2����) MAKEWORD(2,2) �ֹ���2 �ι���2(2.2����)
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		Error_handling("WSAStartup() error!");
	}
	//���� ����
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) {
		Error_handling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	//SOCKET_ERROR�� -1�� define�Ǿ��ְ� bind�� IP�� ��Ʈ��ȣ�� �Ҵ��ϴ� �Լ��̸� ������ 0 ���н� -1 Return
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		Error_handling("bind() error");
	}

	//listen�� ������ �����û�� ������ ���°� �ǰԲ� �ϴ°� �����ϸ� 0 ���н� -1
	if (listen(hServSock, 5) == SOCKET_ERROR) {
		Error_handling("listen() error");
	}

	szClntAddr = sizeof(clntAddr);
	//accept�� ����䫊�� ���� ����
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET) {
		Error_handling("accept() error");
	}

	send(hClntSock, message, sizeof(message), 0);
	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup(); //���Ӱ��� �Լ� ȣ�� ���̻� ���ϰڴٴ� �ǹ�
	return 0;
}

void Error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}