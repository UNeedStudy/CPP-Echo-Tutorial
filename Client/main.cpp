#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) //서버와 마찬가지로 오류를 처리하는 함수 있음
{
	cout << "[오류 발생]: " << message << '\n';
	system("pause");
	exit(1);
}

int main()
{
	WSADATA wsaData;
	SOCKET clientSocket; //서버와 연결된 자기자신의 소켓을 의미
	SOCKADDR_IN serverAddress; //서버의 주소를 담기 위해 sockaddress 구조체를 이용해 하나의 변수 만듦

	int serverPort = 9876; //포트번호가 서버와 동일해야함
	char received[256];
	string sent; //사용자가 입력한 문자열을 서버로 보내주기 위해 하나의 스트링 변수를 만들어줌

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //Winsock 초기화
		ShowErrorMessage("WSAStartup()");

	clientSocket = socket(PF_INET, SOCK_STREAM, 0); //TCP 소켓 생성

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); //서버 주소를 0으로 초기화
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1"); //서버의 주소를 전달하기 위해 inet_addr()함수 불러옴
	// inet_addr 함수는 내부적으로 매개변수로 문자열을 입력받음
	// 127.0.0.1 은 자기 자신의 컴퓨터를 의미하는 것
	// 이 함수를 바로 불러오려면 winsock deprecated no warnings를 적어야함
	serverAddress.sin_port = htons(serverPort);
	if (connect(clientSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("connect()"); //클라이언트는 connet를 사용해 서버에게 연결 요청을 할 수 있음 서버는 accept함수로 받음
	cout << "[현재 상태] connect()\n";

	while (1) { //연결이 성공 되었다면 반복적으로 서버로 메시지를 전송하고 에코메시지를 전달 받음
		cout << "[메시지 전송]: ";
		getline(cin, sent);
		if (sent == "") continue; //사용자가 입력하지않고 엔터만 친 경우는 무시함
		send(clientSocket, sent.c_str(), sent.length(), 0); //특정한 문자를 입력한 경우는 입력한 문자를 클라이언트 소켓을 이용해
		//서버로 보냄 sent는 문자열 변수인 string으로 만들었기 때문에 c언어의 문자열 버퍼 형태로 바꾸려면 c_str()함수를 불러와야함
		//length 함수를 사용해 보내고자하는 문자열의 길이만큼 서버로 보냄
		int length = recv(clientSocket, received, sizeof(received), 0);
		//서버로부터 에코 처리된 문자열이 날라올거기 때문에 recv로 서버로부터 받은 문자열을 received라는 문자열 버퍼에 담음
		received[length] = '\0'; //문자열버퍼 마지막엔 널값을 넣어 문자열 버퍼를 효과적으로 처리하도록 함
		if (strcmp(received, "[exit]") == 0) { //서버로부터 전달받은 값이 exit라면 프로그램을 종료하도록 함
			cout << "[서버종료]\n";
			break;
		}
		cout << "[서버 메시지]: " << received << '\n'; //그렇지 않다면 서버로부터 전달받은 메시지를 그대로 출력할 수 있도록 함
	}

	closesocket(clientSocket);
	WSACleanup();//winsock 사용하지않겠다고 선언
	return 0;
}

//솔루션에서 다시 빌드 버튼을 눌러 솔루션 단위로 빌드를 진행해 실행해 볼 수 있음
// 빌드가 끝나면 파일탐색기에서 폴더열기를 누르고 debug폴더에 가보면 빌드가 완료된 것을 볼 수 있음