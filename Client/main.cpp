#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) //������ ���������� ������ ó���ϴ� �Լ� ����
{
	cout << "[���� �߻�]: " << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET clientSocket; //������ ����� �ڱ��ڽ��� ������ �ǹ�
	SOCKADDR_IN serverAddress; //������ �ּҸ� ��� ���� sockaddress ����ü�� �̿��� �ϳ��� ���� ����

	int serverPort = 9876; //��Ʈ��ȣ�� ������ �����ؾ���
	char received[256];
	string sent; //����ڰ� �Է��� ���ڿ��� ������ �����ֱ� ���� �ϳ��� ��Ʈ�� ������ �������

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //Winsock �ʱ�ȭ
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); //TCP ���� ����

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); //���� �ּҸ� 0���� �ʱ�ȭ
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //������ �ּҸ� �����ϱ� ���� inet_addr()�Լ� �ҷ���
	// inet_addr �Լ��� ���������� �Ű������� ���ڿ��� �Է¹���
	// 127.0.0.1 �� �ڱ� �ڽ��� ��ǻ�͸� �ǹ��ϴ� ��
	// �� �Լ��� �ٷ� �ҷ������� winsock deprecated no warnings�� �������
	serverAddress.sin_port = htons(serverPort);
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()"); //Ŭ���̾�Ʈ�� connet�� ����� �������� ���� ��û�� �� �� ���� ������ accept�Լ��� ����
	cout << "[���� ����] connect()\n";

	while (1) { //������ ���� �Ǿ��ٸ� �ݺ������� ������ �޽����� �����ϰ� ���ڸ޽����� ���� ����
		cout << "[�޽��� ����]: ";
		getline(cin, sent);
		if (sent == "") continue; //����ڰ� �Է������ʰ� ���͸� ģ ���� ������
		send(clientSocket, sent.c_str(), sent.length(), 0); //Ư���� ���ڸ� �Է��� ���� �Է��� ���ڸ� Ŭ���̾�Ʈ ������ �̿���
		//������ ���� sent�� ���ڿ� ������ string���� ������� ������ c����� ���ڿ� ���� ���·� �ٲٷ��� c_str()�Լ��� �ҷ��;���
		//length �Լ��� ����� ���������ϴ� ���ڿ��� ���̸�ŭ ������ ����
		int length = recv(clientSocket, received, sizeof(received), 0);
		//�����κ��� ���� ó���� ���ڿ��� ����ðű� ������ recv�� �����κ��� ���� ���ڿ��� received��� ���ڿ� ���ۿ� ����
		received[length] = '\0'; //���ڿ����� �������� �ΰ��� �־� ���ڿ� ���۸� ȿ�������� ó���ϵ��� ��
		if (strcmp(received, "[exit]") == 0) { //�����κ��� ���޹��� ���� exit��� ���α׷��� �����ϵ��� ��
			cout << "[��������]\n";
			break;
		}
		cout << "[���� �޽���]: " << received << '\n'; //�׷��� �ʴٸ� �����κ��� ���޹��� �޽����� �״�� ����� �� �ֵ��� ��
	}

	closesocket(clientSocket);
	WSACleanup();//winsock ��������ʰڴٰ� ����
	return 0;
}

//�ַ�ǿ��� �ٽ� ���� ��ư�� ���� �ַ�� ������ ���带 ������ ������ �� �� ����
// ���尡 ������ ����Ž���⿡�� �������⸦ ������ debug������ ������ ���尡 �Ϸ�� ���� �� �� ����