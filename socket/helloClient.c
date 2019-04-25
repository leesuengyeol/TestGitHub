//Client 만들기


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
	fputc('\n',stderr);
	exit(1);
}


int main(int argc, char* argv[])
{
	int sock; // fd
	struct sockaddr_in serv_addr; //주소
	char message[MSG_SIZE];	//메세지
	int str_len;

	if(argc != 3) //3개의 인자를 받을것
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
	sock = socket(PF_INET,SOCK_STREAM, 0);
	if (sock==-1)
			error_handling("socket() error");

	
	//STEP 2. 접속할 서버의 IP주소 , 포트번호, 프로토콜을 정의
	//struct sockaddr_in 사용
	
	//구조체 초기화
	memset(&serv_addr, 0,sizeof(serv_addr)); 

	//ip v4 address family
	serv_addr.sin_family=AF_INET;
	
	//주소 지정
	//inet_addr =문자열 어드레스를 inet_addr로 변환해줌
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	
	
	//포트 번호 지정
	//문자열 포트번호를 정수로 변환후
	//htons = host to network 로 port번호 변환하는데 short로 변환
	serv_addr.sin_port=htons(atoi(argv[2]));

	//STEP 3. 접속 요청
	// 	sock: 클라이언트 소켓의 파일 디스크립터
	//	&serv_addr : 연결 요청을 보낼 서버의 주소 정보를 지닌 구조체 변수의 포인터
	//  sizeof(serv_addr) : serv_addr 포인터가 가리키는 주소 정보 구조체 변수의 크기
	if(connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
	{
		error_handling("connect() error!");
		
	}

	

	// STEP 4. 데이터 수신
	//read함수 사용 sizeof -1 은 널문자는 가져오지 않음
	str_len = read(sock, message, sizeof(message) -1);
	if(str_len == -1)
			error_handling("read() error!");

	printf("Message from server: %s\n", message);

	//STEP 5. 소켓 종료
	close(sock);

	return 0;


}
