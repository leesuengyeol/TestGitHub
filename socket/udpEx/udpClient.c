//udp client 만들기

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//네트워크
#include <arpa/inet.h> // 소켓사용시 필요
#include <sys/socket.h> 



#define MSG_SIZE	100

//error 메시지 출력함수
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int sock; // fd
	struct sockaddr_in serv_addr, from_addr; //주소
	char message[MSG_SIZE];	//메세지
	int str_len;
	socklen_t adr_sz;

	if (argc != 3) //3개의 인자를 받을것
	{
		// 받아야할 인자
		// ./helloClient [SERVER_IDADDR] [PORT_NUM]
		printf("Usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	//STEP 1. socket을 생성한다. file의 open과 동일

	// PF_INET : IP v4
	// SOCK_STREAM : TCP
	//IPPROTO_TCP : TCP 사용 하지만 앞에 SCOK_STREAM으로 TCP를 정의했으므로 0을 적어도 된다
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");


	//STEP 2. 접속할 서버의 IP주소 , 포트번호, 프로토콜을 정의
	//struct sockaddr_in 사용

	//구조체 초기화
	memset(&serv_addr, 0, sizeof(serv_addr));

	//ip v4 address family
	serv_addr.sin_family = AF_INET;

	//주소 지정
	//inet_addr =문자열 어드레스를 inet_addr로 변환해줌
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);


	//포트 번호 지정
	//문자열 포트번호를 정수로 변환후
	//htons = host to network 로 port번호 변환하는데 short로 변환
	serv_addr.sin_port = htons(atoi(argv[2]));

	while (1)
	{
		fputs("Insert message(q to quit):",stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		sendto(sock, message, strlen(message),0,(struct sockaddr*)&serv_addr, sizeof(serv_addr)); //strlen 널문자 빼고 길이
		
		adr_sz = sizeof(from_addr); //사이즈의 주소값을 넘겨주기위해

		//from_addr은 자동으로 보내는쪽에서 정보를 보내준다
		str_len = recvfrom(sock, message, MSG_SIZE, 0, (struct sockaddr*)&from_addr, &adr_sz); 
		message[str_len] = 0;
		printf("Message from server : %s", message);
	}
	close(sock);
	return 0;

}
