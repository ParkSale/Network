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
	//WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData) : (윈속의 버전정보,구조체 변수의 주소 값 전달)
	//MAKEWORD(1,2) : 주버전1 부버전2 (1.2버전) MAKEWORD(2,2) 주버전2 부버전2(2.2버전)
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		Error_handling("WSAStartup() error!");
	}
	//소켓 생성
	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET) {
		Error_handling("socket() error");
	}

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	//SOCKET_ERROR은 -1로 define되어있고 bind는 IP와 포트번호를 할당하는 함수이며 성공시 0 실패시 -1 Return
	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
		Error_handling("bind() error");
	}

	//listen은 소켓을 연결요청이 가능한 상태가 되게끔 하는것 성공하면 0 실패시 -1
	if (listen(hServSock, 5) == SOCKET_ERROR) {
		Error_handling("listen() error");
	}

	szClntAddr = sizeof(clntAddr);
	//accept는 연결요쳥에 대한 수락
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET) {
		Error_handling("accept() error");
	}

	send(hClntSock, message, sizeof(message), 0);
	closesocket(hClntSock);
	closesocket(hServSock);
	WSACleanup(); //윈속관련 함수 호출 더이상 안하겠다는 의미
	return 0;
}

void Error_handling(char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}