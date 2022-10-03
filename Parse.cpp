#include "Parse.h"

std::string Parse::parseFirstLine(std::ostringstream& oss, char* header, size_t hsize) {
	//check if get request
	if (UTIL::containedBeforeNewline(header, "GET", hsize)) {
		//construct a bit of response header
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
		return "";
	}
	else {
		return "";
	}
}

//craft response headet through this
Command Parse::parseHeader(char* header, size_t hsize) {
	std::ostringstream oss;
	//read through header data and return enum command to run	
	std::string loc = parseFirstLine(oss, header, hsize);
	if (loc.compare("") == 0) {
		Command c;
		c.run = COM::ERR;
		return c;
	}
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
//https://www.freeformatter.com/mime-types-list.html
void Parse::generateFileTypes() {
	//figure out way to just have this stored in some file; maybe scrape some site
	FileType e1;
	e1.file_end = ".html";
	e1.write_type = "text/html";
	types.push_back(e1);
	e1.file_end = ".png";
	e1.write_type = "image/png";
	types.push_back(e1);
	e1.file_end = ".zip";
	e1.write_type = "application/zip";
	types.push_back(e1);
}

void Parse::loadMimeFile() {
	std::ifstream file;
	file.open("mime.dat");
	if (!file) {
		return;
	}
	//read file and parse each line
	std::ostringstream oss;
	oss << file.rdbuf();
	file.close();
	std::string s = oss.str();
	FileType e1;
	//switch this for write_type or file_end
	bool type = true;
	std::string write_type;
	std::string end;
	int c = 0;
	for (auto& i : s) {
		if (type) {
			if (i != ':') {
				write_type.push_back(i);
			}
			else {
				type = false;
				c = 0;
			}
		}
		else {
			if (i != '\n' && i != '.') {
				end.push_back(i);
			}
			else if (i == '.') {
				c++;
				if (c > 1) {
					e1.file_end = end;
					e1.write_type = write_type;
					types.push_back(e1);
					end.clear();
				}
				end.push_back(i);
			}
			else {
				e1.file_end = end;
				e1.write_type = write_type;
				types.push_back(e1);
				end.clear();
				write_type.clear();
				type = true;
			}
		}
	}
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

void Config::readConfig() {
	std::ifstream file;
	file.open("config.dat");
	if (!file) {
		generateConfig();
		file.open("config.dat");
	}
	//first line contains port
	char c;
	while (file.get(c) && c != ':'); //getting to point right before port num
	while (file.get(c) && !std::isdigit(c));//getting past any spaces
	std::string num;
	num.push_back(c);
	while (file.get(c) && std::isdigit(c) && c != '\n') {
		num.push_back(c);
	}
	port = std::atoi(num.c_str());
	//second line contains list of allowed files
	std::string path;
	while (file.get(c) && c != '\n') {
		if (c == ':') {
			allowed_paths.push_back(path);
			path.clear();
		}
		else {
			path.push_back(c);
		}
	}
}

void Config::generateConfig() {
	std::ofstream file;
	file.open("config.dat");
	file << "port: 80\nindex_files/:\n";
	file.close();
}
//causing server to ignore all requests even when should be allowed
bool Config::checkPath(std::string loc) {
	if (loc.find("/") == std::string::npos) {
		return true;
	}
	std::string path = UTIL::extractPath(loc);
	for (auto& i : allowed_paths) {
		if (i.find(path) != std::string::npos) {
			return true;
		}
	}
	return false;
}