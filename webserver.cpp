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
	//FileBreak br("index_files/temp.txt", 41);
	//std::string out1 = br.getNextChunk();
	//std::string out2 = br.getNextChunk();
	Server serv;
	bool exitf = false;
	while (!exitf) {
		serv.update();
	}
	return 0;
}