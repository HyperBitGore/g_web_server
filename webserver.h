#pragma once
#include "Parse.h"
#include <thread>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")

//add all mime types; use a big file, packaged with release so you can add mime types if you want
//implement POST


//https://developer.mozilla.org/en-US/docs/Glossary/HTTP_header
class Server {
private:
	Parse parser;
	char buf[8192];
	fd_set master;
	SOCKET listener;
	Config cof;
	FileBreak fb;
	//only return identity encoding for now
	//send html data back to socket, or whatever you want
	bool GET_Request(SOCKET sock, std::string path, std::string start) {
		//get file size and decide whether to send multiple packets; if greater than 1mb send multiple packets
		int size = parser.getFileSize(path);
		//if file size is too big for one http packet, send multiple
		if (size > (1048576)) {
			fb.open(path, size);
			//send intial packet with header, and then send the rest of the file in 1mb packets
			std::ostringstream oss;
			oss << start;
			oss << "Accept-Ranges: bytes\r\n";
			//https://www.iana.org/assignments/media-types/media-types.xhtml
			//need to categorize file types like this
			parser.categorizeFile(oss, path);
			oss << "Content-Length: " + std::to_string(size) + "\r\n";
			oss << "\r\n";
			oss << fb.getNextChunk();
			//send requested file over socket
			//we don't add a trailing zero byte here because it corrupts data
			send(sock, oss.str().c_str(), oss.str().size(), 0);
			oss.clear();
			std::string input;
			while ((input = fb.getNextChunk()).compare("") != 0) {
				send(sock, input.c_str(), input.size(), 0);
			}
			fb.close();
			return true;
		}
		else {
			//put request file in memory
			std::stringstream sstream;
			std::ifstream f;
			f.open(path.c_str(), std::ios::binary);
			sstream << f.rdbuf();
			std::string file = sstream.str();
			//create response
			std::ostringstream oss;
			oss << start;
			//https://www.iana.org/assignments/media-types/media-types.xhtml
			//need to categorize file types like this
			parser.categorizeFile(oss, path);
			oss << "Content-Length: " + std::to_string(file.size()) + "\r\n";
			oss << "\r\n";
			oss << file;
			//send requested file over socket
			send(sock, oss.str().c_str(), oss.str().size() + 1, 0);
			oss.clear();
			return false;
		}
	}

public:
	Server() {
		cof.readConfig();
		FD_ZERO(&master);
		//create listening socket and add to fd_set master
		listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (listener == INVALID_SOCKET) {
			std::cerr << "Listener socket creation failed " << std::endl;
		}
		sockaddr_in hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(cof.getPort());
		hint.sin_addr.S_un.S_addr = INADDR_ANY;
		bind(listener, (sockaddr*)&hint, sizeof(hint));
		//tells winsock that the socket is for listening
		listen(listener, SOMAXCONN);
		FD_SET(listener, &master);
		//generate file types
		parser.generateFileTypes();
		std::cout << "Started server on port " << cof.getPort() << std::endl;

	}
	void runCommand(Command com, SOCKET sock) {
		switch (com.run) {
		case COM::GET:
			GET_Request(sock, com.location, com.header);
			break;
		}
	}
	void update() {
		//listen for connections and send html data when get request given
		fd_set copy = master;
		int sockCount = select(0, &copy, nullptr, nullptr, nullptr);
		for (int i = 0; i < sockCount; i++) {
			SOCKET sock = copy.fd_array[i];
			if (sock == listener) {
				//add connection to master
				sockaddr_in client;
				int clientsize = sizeof(client);
				SOCKET clientsocket = accept(sock, (sockaddr*)&client, &clientsize);
				if (clientsocket == INVALID_SOCKET) {
					std::cerr << "Client connect failed" << std::endl;
				}
				char host[NI_MAXHOST]; //clients remote name
				char service[NI_MAXHOST]; //service (port) client is connection on

				ZeroMemory(host, NI_MAXHOST);
				ZeroMemory(host, NI_MAXHOST);

				if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
					std::cout << host << " connected on port " << service << std::endl;
				}
				else {
					inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
					std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
				}
				//might need to do reply here, but we'll see
				FD_SET(clientsocket, &master);
			}
			else {
				ZeroMemory(buf, 8192);
				//parse header sent by user
				int bytes = recv(sock, buf, 8192, 0);
				if (bytes <= 0) {
					//disconnect and remove from master
					closesocket(sock);
					FD_CLR(sock, &master);
					std::cout << "User disconnected\n";
				}
				else {
					//libre wolf sends https request so we can't read
					//parse the header from user and then run whatever command it sent
					Command c = parser.parseHeader(buf, bytes);
					//check if location is allowed
					if (c.run != COM::ERR && cof.checkPath(c.location)) {
						runCommand(c, sock);
					}
					//disconnect and remove from master
					closesocket(sock);
					FD_CLR(sock, &master);
					std::cout << "User disconnected\n";
				}
			}
		}

	}

};

//send commands to server on seperate thread????
class CommandHandler {
private:

public:

};