#pragma once
#include "Utility.h"
#include <sstream>
#include <vector>

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