//Client �����


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//��Ʈ��ũ
#include <arpa/inet.h> // ���ϻ��� �ʿ�
#include <sys/socket.h> 

//#define DEBUG

struct calcul
{
	int opnd_cnt; //�ǿ����� ����
	int opnds[4]; //�ǿ�����
	char oper; //������
};

//error �޽��� ����Լ�
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
	int i; //for����
	struct calcul cal;
	struct sockaddr_in serv_addr; //�ּ�
	
	int str_len; //������ ����� �޾Ҵ��� Ȯ��

	if (argc != 3) //3���� ���ڸ� ������
	{
		// �޾ƾ��� ����
		// ./helloClient [SERVER_IDADDR] [PORT_NUM]
		printf("Usage : %s <IP> <PORT>\n", argv[0]);
		exit(1);
	}

	//���ϻ���
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock == -1)
		error_handling("socket() error");

	//������ ������ IP�ּ� , ��Ʈ��ȣ ,���������� ����

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

	//���� ��û
	if (connect(sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
	{
		error_handling("connect() error!");

	}

	//�ʿ� ���� �ޱ�

	printf("Operand count:");
	scanf("%d", &cal.opnd_cnt);

	for (i = 0; i < cal.opnd_cnt; i++)
	{
		printf("Operand %d: ",i+1);
		scanf("%d", &cal.opnds[i]);
	}
	getchar(); //����Ű �����°� ����

	printf("Operation:");
	scanf("%c", &cal.oper);

#ifdef DEBUG

	printf("opnd_cnt: %d\n", cal.opnd_cnt);
	printf("opnd[0]:%d\n",cal.opnds[0]);
	printf("opnd[1]:%d\n",cal.opnds[1]);
	printf("opnd[2]:%d\n",cal.opnds[2]);
	printf("oper:%c\n",cal.oper);

#endif

	//����ü ����
	write(sock, &cal, sizeof(cal));

	//����� �ޱ�
	read(sock, &result, sizeof(result));

	printf("result : %d \n", result);
	//STEP 5. ���� ����
	close(sock);

	return 0;
}