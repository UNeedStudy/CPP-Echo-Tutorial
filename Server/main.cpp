#include <iostream>
#include <string>
#include <WinSock2.h>

using namespace std;

void ShowErrorMessage(string message) //네트워크 관련 함수는 에러가 많이 발생할 수 있기 때문에 에러 처리함수가 꼭 필요
{
	cout << "[오류발생]" << message << '\n';
	system("pause");
	exit(1); //에러가 났다면 오류발생이라는 메시지를 띄우고 프로그램 바로 종료
}

int main()
{
	WSADATA wsaData; //윈속 api 데이터 구조체를 하나의 변수로 만들어줌
	SOCKET serverSocket, clientSocket; //클라이언트 소켓과 서버 소켓을 변수로 만듦
	SOCKADDR_IN serverAddress, clientAddress; //서버주소와 클라이언트 주소를 정보를 담고 처리할 수 있도록 하는 소켓 어드레스 구조체 설정

	int serverPort = 9876; //서버포트 설정
	char received[256]; //클라이언트와 데이터를 주고 받을 때 문자열 버퍼의 크기를 256으로 설정

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) //winsock을 2.2버전을 사용하겠다고 하는 것으로 초기화
		ShowErrorMessage("WSAStartup()"); //startup된 결과를 winsock api 데이터에서 확인할 수 있도록 처리
	// 0이 아니라면 오류가 발생했다고 메시지 출력

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); //서버 소켓 초기화 , TCP 소켓 생성
	// PF_INET, AF_INET은 IPv4를 사용하겠다고 명시한 것
	// SOCK_STREAM은 TCP를 의미

	if (serverSocket == INVALID_SOCKET)  //예외처리
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress)); //서버 어드레스를 0으로 초기화 함
	serverAddress.sin_family = AF_INET; //어떤 체계를 사용할 건지 명시
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY); // 서버 본연의 주소로 자신의 주소를 사용해 서버를 열겠다고 서버 값을 넣어줌
	// htonl 은(l = long) 4바이트 정수를 네트워크 바이트 형식으로 담아준다는 뜻
	serverAddress.sin_port = htons(serverPort); //htons 의 s는 short. 2바이트 정수를 네트워크 바이트 형식으로 포트번호를 넣어준다는 뜻
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	//해당 서버 소켓으로 바인드 처리를 해서 실제로 서버 프로그램을 실행하고 있는 컴퓨터에 네트워크 인터페이스에 바인딩이 될 수 있도록 처리
	// 바인딩 과정을 위해 서버 주소가 들어가 있는 것
	cout << "[현재상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR) //동시에 연결을 요청하는 클라이언트의 수가 총 5개까지 들어갈 수 있도록 함
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);
	// Accept함수를 수행해서 클라이언트의 요청을 기다릴 것
	// 클라이언트의 대한 정보가 clientaddress에 담기게 될 것
	clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
	//해당 클라이언트와 통신할 수 있는 소켓은 clientsocket에 들어가게 됨
	cout << "[현재상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) { //클라이언트의 메시지를 받아 그대로 다시 전달
		//서버 프로그램은 무한 반복. 클라이언트가 서버에 접속한 경우 무한 반복하면서 클라이언트로부터 데이터를 전달 받아 그 전달받은
		//메시지를 다시 클라이언트에 보내줌 
		int length = recv(clientSocket, received, sizeof(received), 0);
		//기본적으로 문자열 버퍼에 클라이언트가 보낸 문자가 담김
		received[length] = NULL;
		//마지막 문자를 NULL로 바꿔 예기치 못한 문자가 들어왔을 때 그것을 막을 수 있도록 함
		cout << "[클라이언트 메시지]: " << received << '\n';
		// 클라이언트가 보낸 메시지를 출력
		cout << "[메시지 전송]: " << received << '\n';
		// 클라이언트에게 동일한 메시지 전송
		if (strcmp(received, "[exit]") == 0) {
			send(clientSocket, received, sizeof(received) - 1, 0);
			// 전송 시 클라이언트가 문자열을 엔터를 쳐서 보내기 때문에 마지막 공백은 제거하도록 함
			// 사용자가 보낸 문자가 exit이라는 명령이라면 서버프로그램을 종료
			cout << "[서버 종료]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
		// 그렇지 않다면 그냥 바로 클라이언트한테 클라이언트가 보낸 메시지를 그대로 보냄 (에코 프로그램)
	}

	closesocket(clientSocket);
	closesocket(serverSocket);
	// 서버와 클라이언트 소켓을 종료
	WSACleanup();
	//더이상 윈속을 사용하지 않겠다고 명시
	system("pause");
	return 0;







}