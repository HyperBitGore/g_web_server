#pragma once
#include "Utility.h"
#include <sstream>
#include <vector>
#include <fstream>

//probably only gonna implement post, get, and put
enum class COM { POST, GET, ERR };

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
	//loads mime type file
	void loadMimeFile();
	//generate file type vectors
	void generateFileTypes();
};


//not sending entirely correct file
class FileBreak {
private:
	std::ifstream file;
	int loc = 0;
	int f_size;
	int chunk_size;
public:
	FileBreak() {
		f_size = 0;
		chunk_size = 1048576;
	}
	FileBreak(std::string loc, int ins, int chunk) {
		file.open(loc.c_str(), std::ios::binary);
		f_size = ins;
		chunk_size = chunk;
	}
	void open(std::string path, int ins, int chunk = 1048576) {
		file.open(path.c_str(), std::ios::binary);
		f_size = ins;
		loc = 0;
		chunk_size = chunk;
	}
	std::string getNextChunk() {
		if (loc >= f_size) {
			return "";
		}
		int c = 0;
		char car;
		std::string out;
		while (c < chunk_size && file.get(car)) {
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
	int max_size;
	std::vector<std::string> allowed_paths;
public:
	void generateConfig();
	void readConfig();
	bool checkPath(std::string loc);
	int getPort() {
		return port;
	}
	int getMaxSize() {
		return max_size;
	}
	std::vector<std::string>& getPaths() {
		return allowed_paths;
	}
};