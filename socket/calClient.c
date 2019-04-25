//Client 만들기


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//네트워크
#include <arpa/inet.h> // 소켓사용시 필요
#include <sys/socket.h> 

//#define DEBUG

struct calcul
{
	int opnd_cnt; //피연산자 갯수
	int opnds[4]; //피연산자
	char oper; //연산자
};

//error 메시지 출력함수
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}


int main(int argc, char* argv[])
{
	int result;
	int sock; // fd
	int i; //for문용
	struct calcul cal;
	struct sockaddr_in serv_addr; //주소
	
	int str_len; //받을때 제대로 받았는지 확인

	if (argc != 3) //3개의 인자를 받을것
	{
		// 받아야할 인자
		// ./helloClient [SERVER_IDADDR] [PORT_NUM]
		printf("Usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	//소켓생성
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	//접속할 서버의 IP주소 , 포트번호 ,프로토콜을 정의

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

	//접속 요청
	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("connect() error!");

	}

	//필요 정보 받기

	printf("Operand count:");
	scanf("%d", &cal.opnd_cnt);

	for (i = 0; i < cal.opnd_cnt; i++)
	{
		printf("Operand %d: ",i+1);
		scanf("%d", &cal.opnds[i]);
	}
	getchar(); //엔터키 씹히는거 방지

	printf("Operation:");
	scanf("%c", &cal.oper);

#ifdef DEBUG

	printf("opnd_cnt: %d\n", cal.opnd_cnt);
	printf("opnd[0]:%d\n",cal.opnds[0]);
	printf("opnd[1]:%d\n",cal.opnds[1]);
	printf("opnd[2]:%d\n",cal.opnds[2]);
	printf("oper:%c\n",cal.oper);

#endif

	//구조체 전송
	write(sock, &cal, sizeof(cal));

	//결과값 받기
	read(sock, &result, sizeof(result));

	printf("result : %d \n", result);
	//STEP 5. 소켓 종료
	close(sock);

	return 0;
}