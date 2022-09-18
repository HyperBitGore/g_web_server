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
	static bool containedBeforeNewline(char* start, std::string word) {
		char* dt = start;
		size_t posword = 0;
		while (*dt != '\n') {
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

}