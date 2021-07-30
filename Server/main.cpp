#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) //��Ʈ��ũ ���� �Լ��� ������ ���� �߻��� �� �ֱ� ������ ���� ó���Լ��� �� �ʿ�
{
	cout << "[�����߻�]" << message << '\n';
	system("pause");
	exit(1); //������ ���ٸ� �����߻��̶�� �޽����� ���� ���α׷� �ٷ� ����
}

int main()
{
	WSADATA wsaData; //���� api ������ ����ü�� �ϳ��� ������ �������
	SOCKET serverSocket, clientSocket; //Ŭ���̾�Ʈ ���ϰ� ���� ������ ������ ����
	SOCKADDR_IN serverAddress, clientAddress; //�����ּҿ� Ŭ���̾�Ʈ �ּҸ� ������ ��� ó���� �� �ֵ��� �ϴ� ���� ��巹�� ����ü ����

	int serverPort = 9876; //������Ʈ ����
	char received[256]; //Ŭ���̾�Ʈ�� �����͸� �ְ� ���� �� ���ڿ� ������ ũ�⸦ 256���� ����

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //winsock�� 2.2������ ����ϰڴٰ� �ϴ� ������ �ʱ�ȭ
		ShowErrorMessage("WSAStartup()"); //startup�� ����� winsock api �����Ϳ��� Ȯ���� �� �ֵ��� ó��
	// 0�� �ƴ϶�� ������ �߻��ߴٰ� �޽��� ���

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); //���� ���� �ʱ�ȭ , TCP ���� ����
	// PF_INET, AF_INET�� IPv4�� ����ϰڴٰ� ����� ��
	// SOCK_STREAM�� TCP�� �ǹ�

	if (serverSocket == INVALID_SOCKET)  //����ó��
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); //���� ��巹���� 0���� �ʱ�ȭ ��
	serverAddress.sin_family = AF_INET; //� ü�踦 ����� ���� ���
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // ���� ������ �ּҷ� �ڽ��� �ּҸ� ����� ������ ���ڴٰ� ���� ���� �־���
	// htonl ��(l = long) 4����Ʈ ������ ��Ʈ��ũ ����Ʈ �������� ����شٴ� ��
	serverAddress.sin_port = htons(serverPort); //htons �� s�� short. 2����Ʈ ������ ��Ʈ��ũ ����Ʈ �������� ��Ʈ��ȣ�� �־��شٴ� ��
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	//�ش� ���� �������� ���ε� ó���� �ؼ� ������ ���� ���α׷��� �����ϰ� �ִ� ��ǻ�Ϳ� ��Ʈ��ũ �������̽��� ���ε��� �� �� �ֵ��� ó��
	// ���ε� ������ ���� ���� �ּҰ� �� �ִ� ��
	cout << "[�������] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) //���ÿ� ������ ��û�ϴ� Ŭ���̾�Ʈ�� ���� �� 5������ �� �� �ֵ��� ��
		ShowErrorMessage("listen()");
	cout << "[���� ����] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	// Accept�Լ��� �����ؼ� Ŭ���̾�Ʈ�� ��û�� ��ٸ� ��
	// Ŭ���̾�Ʈ�� ���� ������ clientaddress�� ���� �� ��
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	//�ش� Ŭ���̾�Ʈ�� ����� �� �ִ� ������ clientsocket�� ���� ��
	cout << "[�������] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) { //Ŭ���̾�Ʈ�� �޽����� �޾� �״�� �ٽ� ����
		//���� ���α׷��� ���� �ݺ�. Ŭ���̾�Ʈ�� ������ ������ ��� ���� �ݺ��ϸ鼭 Ŭ���̾�Ʈ�κ��� �����͸� ���� �޾� �� ���޹���
		//�޽����� �ٽ� Ŭ���̾�Ʈ�� ������ 
		int length = recv(clientSocket, received, sizeof(received), 0);
		//�⺻������ ���ڿ� ���ۿ� Ŭ���̾�Ʈ�� ���� ���ڰ� ���
		received[length] = NULL;
		//������ ���ڸ� NULL�� �ٲ� ����ġ ���� ���ڰ� ������ �� �װ��� ���� �� �ֵ��� ��
		cout << "[Ŭ���̾�Ʈ �޽���]: " << received << '\n';
		// Ŭ���̾�Ʈ�� ���� �޽����� ���
		cout << "[�޽��� ����]: " << received << '\n';
		// Ŭ���̾�Ʈ���� ������ �޽��� ����
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			// ���� �� Ŭ���̾�Ʈ�� ���ڿ��� ���͸� �ļ� ������ ������ ������ ������ �����ϵ��� ��
			// ����ڰ� ���� ���ڰ� exit�̶�� ����̶�� �������α׷��� ����
			cout << "[���� ����]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
		// �׷��� �ʴٸ� �׳� �ٷ� Ŭ���̾�Ʈ���� Ŭ���̾�Ʈ�� ���� �޽����� �״�� ���� (���� ���α׷�)
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	// ������ Ŭ���̾�Ʈ ������ ����
	WSACleanup();
	//���̻� ������ ������� �ʰڴٰ� ���
	system("pause");
	return 0;







}