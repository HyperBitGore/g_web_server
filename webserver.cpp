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
	std::ifstream file("/smed/temp.txt");
	std::ostringstream ss;
	ss << file.rdbuf();
	std::cout << ss.str() << std::endl;
	Server serv;
	bool exitf = false;
	while (!exitf) {
		serv.update();
	}
	return 0;
}