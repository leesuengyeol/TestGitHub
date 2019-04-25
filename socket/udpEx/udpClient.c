//udp client �����

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//��Ʈ��ũ
#include <arpa/inet.h> // ���ϻ��� �ʿ�
#include <sys/socket.h> 



#define MSG_SIZE	100

//error �޽��� ����Լ�
void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

int main(int argc, char* argv[])
{
	int sock; // fd
	struct sockaddr_in serv_addr, from_addr; //�ּ�
	char message[MSG_SIZE];	//�޼���
	int str_len;
	socklen_t adr_sz;

	if (argc != 3) //3���� ���ڸ� ������
	{
		// �޾ƾ��� ����
		// ./helloClient [SERVER_IDADDR] [PORT_NUM]
		printf("Usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	//STEP 1. socket�� �����Ѵ�. file�� open�� ����

	// PF_INET : IP v4
	// SOCK_STREAM : TCP
	//IPPROTO_TCP : TCP ��� ������ �տ� SCOK_STREAM���� TCP�� ���������Ƿ� 0�� ��� �ȴ�
	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (sock == -1)
		error_handling("socket() error");


	//STEP 2. ������ ������ IP�ּ� , ��Ʈ��ȣ, ���������� ����
	//struct sockaddr_in ���

	//����ü �ʱ�ȭ
	memset(&serv_addr, 0, sizeof(serv_addr));

	//ip v4 address family
	serv_addr.sin_family = AF_INET;

	//�ּ� ����
	//inet_addr =���ڿ� ��巹���� inet_addr�� ��ȯ����
	serv_addr.sin_addr.s_addr = inet_addr(argv[1]);


	//��Ʈ ��ȣ ����
	//���ڿ� ��Ʈ��ȣ�� ������ ��ȯ��
	//htons = host to network �� port��ȣ ��ȯ�ϴµ� short�� ��ȯ
	serv_addr.sin_port = htons(atoi(argv[2]));

	while (1)
	{
		fputs("Insert message(q to quit):",stdout);
		fgets(message, sizeof(message), stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		sendto(sock, message, strlen(message),0,(struct sockaddr*)&serv_addr, sizeof(serv_addr)); //strlen �ι��� ���� ����
		
		adr_sz = sizeof(from_addr); //�������� �ּҰ��� �Ѱ��ֱ�����

		//from_addr�� �ڵ����� �������ʿ��� ������ �����ش�
		str_len = recvfrom(sock, message, MSG_SIZE, 0, (struct sockaddr*)&from_addr, &adr_sz); 
		message[str_len] = 0;
		printf("Message from server : %s", message);
	}
	close(sock);
	return 0;

}
