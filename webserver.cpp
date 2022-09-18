#include "webserver.h"



int main() {
	WSADATA wsdat;
	WORD ver = MAKEWORD(2, 2);
	srand(time(NULL));
	//start winsock
	int wsOK = WSAStartup(ver, &wsdat);
	if (wsOK != 0) {
		std::cerr << "Can't init winsock!" << std::endl;
		return -1;
	}
	Server serv;
	bool exitf = false;
	while (!exitf) {
		serv.update();
	}
	return 0;
}