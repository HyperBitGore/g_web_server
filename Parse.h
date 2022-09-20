#pragma once
#include "Utility.h"
#include <sstream>

//probably only gonna implement post, get, and put
enum class COM { POST, GET, HEAD, PUT, DEL, CONNECT, OPTIONS, TRACE };

struct Command {
	COM run;
	std::string header;
	std::string location;
};




class Parse {
private:

public:
	//static std::string parseGET(std::ostringstream& oss, char* header);

	static std::string parseFirstLine(std::ostringstream& oss, char* header, size_t hsize);

	//craft response headet through this
	static Command parseHeader(char* header, size_t hsize);
};