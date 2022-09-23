#include "Parse.h"
#include <fstream>

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
		else {
			loc.erase(loc.begin());
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



void Parse::categorizeFile(std::ostringstream& oss, std::string file) {
	for (auto& i : types) {
		if (file.find(i.file_end.c_str()) != std::string::npos) {
			oss << "Content-Type: " + i.write_type + "\r\n";
			return;
		}
	}

}
//https://www.sitepoint.com/mime-types-complete-list/
//https://mimetype.io/all-types/
void Parse::generateFileTypes() {
	//figure out way to just have this stored in some file; maybe scrape some site
	FileType e1;
	e1.file_end = ".html";
	e1.write_type = "text/html";
	types.push_back(e1);
	e1.file_end = ".png";
	e1.write_type = "image/png";
	types.push_back(e1);
}


int Parse::getFileSize(std::string file) {
	std::ifstream f;
	f.open(file.c_str(), std::ios::binary);
	//read through file line by line and count up each byte
	int c = 0;
	std::string line;
	while (getline(f, line, '\n')) {
		for (auto& i : line) {
			c++;
		}
		c++;
	}
	c--;
	return c;
}