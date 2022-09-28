#pragma once
#include <iostream>
#include <string>




namespace UTIL{
	//finds first occurence of word in char* data block, returns nullptr if unable to find
	static char* findWord(char* data, std::string word, size_t size) {
		size_t c = 0;
		char* dt = data;
		size_t posword = 0;
		while (c < size) {
			if (*dt == word[posword]) {
				posword++;
				if (posword >= word.size()) {
					while (posword >= 0) {
						dt--;
						posword--;
					}
					return dt;
				}
			}
			dt++;
			c++;
		}
		return nullptr;
	}
	//make sure these are both in same block of data, or else you will get out of bounds error or corrupt stack, and that start is before end
	static bool containedBetween(char* start, char* end, std::string word) {
		char* dt = start;
		size_t posword = 0;
		while (dt != end) {
			if (*dt == word[posword]) {
				posword++;
				if (posword >= word.size()) {
					return true;
				}
			}
			dt++;
		}
		return false;
	}
	//make sure you're searching in something with newlines or else will go out of bounds
	static bool containedBeforeNewline(char* start, std::string word, size_t size) {
		char* dt = start;
		size_t c = 0;
		size_t posword = 0;
		while (c < size && *dt != '\n' && posword < word.size()) {
			if (*dt == word[posword]) {
				posword++;
			}
			c++;
			dt++;
		}
		if (posword >= word.size()) {
			return true;
		}
		return false;
	}
	static int countOccurence(std::string str, char c) {
		int n = 0;
		for (auto& i : str) {
			if (i == c) {
				n++;
			}
		}
		return n;
	}
	static std::string extractPath(std::string path) {
		//get number of /'s
		int n = countOccurence(path, '/');
		//loop through till we hit last /
		std::string ret;
		for (auto& i : path) {
			ret.push_back(i);
			if (i == '/') {
				n--;
				if (n <= 0) {
					return ret;
				}
			}
		}
		return ret;
	}
}


