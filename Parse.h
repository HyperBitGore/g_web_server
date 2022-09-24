#pragma once
#include "Utility.h"
#include <sstream>
#include <vector>
#include <fstream>

//probably only gonna implement post, get, and put
enum class COM { POST, GET, HEAD, PUT, DEL, CONNECT, OPTIONS, TRACE };

struct Command {
	COM run;
	std::string header;
	std::string location;
};
struct FileType {
	std::string file_end;
	std::string write_type;
};



class Parse {
private:
	std::vector<FileType> types;
public:
	//static std::string parseGET(std::ostringstream& oss, char* header);

	std::string parseFirstLine(std::ostringstream& oss, char* header, size_t hsize);

	//craft response headet through this
	Command parseHeader(char* header, size_t hsize);

	//get file size
	int getFileSize(std::string file);
	//categorize the file ending
	void categorizeFile(std::ostringstream& oss, std::string file);
	//generate file type vectors
	void generateFileTypes();
};


//not sending entirely correct file
class FileBreak {
private:
	std::ifstream file;
	int loc = 0;
	int f_size;
public:
	FileBreak(std::string loc, int ins) {
		file.open(loc.c_str(), std::ios::binary);
		f_size = ins;
	}
	std::string getNextChunk() {
		if (loc >= f_size) {
			return "";
		}
		int c = 0;
		char car;
		std::string out;
		while (c < 1048576 && file.get(car)) {
			out.push_back(car);
			c++;
			loc++;
		}
		return out;
	}
	void close() {
		file.close();
	}
};


//will be expanded past this
class Config {
private:
	int port;
	std::vector<std::string> allowed_paths;
public:
	void generateConfig();
	void readConfig();
	int getPort() {
		return port;
	}
	std::vector<std::string>& getPaths() {
		return allowed_paths;
	}
};