#include "Parse.h"




std::string Parse::parseFirstLine(std::ostringstream& oss, char* header, size_t hsize) {
	//check if get request
	if (UTIL::containedBeforeNewline(header, "GET", hsize)) {
		oss << "HTTP/1.1 200 OK\r\n";
		oss << "Cache-Control: no-cache, private\r\n";
		oss << "Content-Encoding: identity\r\n";
		oss << "Server: GoreServer\r\n";
		//now find the location the get request wants and return that
		char* dt = header;
		while (*dt != '/') {
			dt++;
		}
		std::string loc;
		while (*dt != ' ') {
			loc.push_back(*dt);
			dt++;
		}
		if (loc.compare("/") == 0) {
			loc = "index.html";
		}
		return loc;
	}
	//check if post request
	else if (UTIL::containedBeforeNewline(header, "POST", hsize)) {
		
	}
}

//craft response headet through this
Command Parse::parseHeader(char* header, size_t hsize) {
	std::ostringstream oss;
	//read through header data and return enum command to run	
	std::string loc = parseFirstLine(oss, header, hsize);
	Command c;
	c.header = oss.str();
	c.location = loc;
	c.run = COM::GET;
	return c;
}