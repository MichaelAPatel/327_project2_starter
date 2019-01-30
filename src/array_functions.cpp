//============================================================================
// Name        : array_functions.cpp
// Author      : Michael Patel & Keith Perkins
// Version     : 1.0
// Description : Array_functions for Project 2, creates array of structs to
//               sort words and occurrences of them from a given input file.
//               Code frame given by Keith Perkins.
//============================================================================

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "utilities.h"
#include "constants.h"
#include "array_functions.h"

// Defining some constants here because we are not allowed to edit the
// constants.h file.
const bool FAILED_TO_OPEN_FILE = false;
const bool SUCCESS = true;

// Defining a structure to track words and number of times they occur.
struct entry{
	std::string word = "";
	int occurrances = 0;
};

// A global array (to this file) of entry structs defined above.
entry words[1024];

// A variable to keep track of next available position in the array.
int nextAvailablePosition = 0;

// Zero out the array and reset the next available position.
void clearArray() {
	nextAvailablePosition = 0;
	for(unsigned int i = 0; i < sizeof(words)/sizeof(words[0]); i++) {
		words[i].word = "";
		words[i].occurrances = 0;
	}
}

// Return how many unique words are stored in the array.
int getArraySize(){
	int size = 0;
	for(unsigned int i = 0; i < sizeof(words)/sizeof(words[0]); i++) {
		if(words[i].word != "") {
			size++;
		}
	}
	return size;
}

// Get the word at a given index.
std::string getArrayWordAt(int i) {
	return words[i].word;
}

// Get the number of occurrences of a word at a given index.
int getArrayWord_NumbOccur_At(int i) {
	return words[i].occurrances;
}

// This function loops through whole file, one line at a time calling processLine for each line
// Returns false if mystream is not open, and true otherwise.
bool processFile(std::fstream & myfstream) {
	if(!myfstream.is_open()){
		return FAILED_TO_OPEN_FILE;
	}
	std::string myString;
	while(std::getline(myfstream, myString)) {
		processLine(myString);
	}
	closeFile(myfstream);
	return SUCCESS;
}

// Takes a line and separates it into tokens, feeding each one into
// the processToken function.
void processLine(std::string & myString) {
	std::stringstream ss(myString);
	std::string tempToken;
	 while(getline(ss, tempToken, constants::CHAR_TO_SEARCH_FOR)) {
		 processToken(tempToken);
	 }
	 return;
}

// Stores each token in a struct, after stripping each one of
// unwanted characters and checking that the words are not already
// in the array.
void processToken(std::string & token) {
	strip_unwanted_chars(token);
	if(token == "") {
		return;
	}
	for(int i = nextAvailablePosition - 1; i >= 0; i--) {
		std::string tempStr1 = token;
		std::string tempStr2 = words[i].word;
		toUpper(tempStr1);
		toUpper(tempStr2);
		if(tempStr1 == tempStr2) {
			words[i].occurrances++;
			return;
		}
	}
	words[nextAvailablePosition].word = token;
	words[nextAvailablePosition].occurrances = 1;
	nextAvailablePosition++;
}

// Attempts to open the filestream, returns FAILED_TO_OPEN_FILE if
// the file does not open, otherwise true.
bool openFile(std::fstream & myfile, const std::string & myFileName,
		std::ios_base::openmode mode) {
	myfile.open(myFileName, mode);
	if(!myfile.is_open()) {
		return FAILED_TO_OPEN_FILE;;
	}
	return SUCCESS;
}

// Closes the filestream if it is open.
void closeFile(std::fstream & myfile) {
	if(myfile.is_open()) {
		myfile.close();
	}
}

// Writes all data in the array to an output file. Returns FAIL_FILE_DID_NOT_OPEN
// if the file fails to open, FAIL_NO_ARRAY_DATA if the array is empty,
// and SUCCESS if all the data is written and the file closes properly.
int writeArraytoFile(const std::string & outputfilename) {
	std::ofstream myfile(outputfilename);
	if(!myfile.is_open()) {
		return constants::FAIL_FILE_DID_NOT_OPEN;
	}
	if(getArraySize() == 0) {
		return constants::FAIL_NO_ARRAY_DATA;
	}
	for(int i = 0; i < getArraySize(); i++) {
		if(words[i].word == "") {
			break;
		}
		myfile << words[i].word << " " << words[i].occurrances << std::endl;
	}
	myfile.close();
	return constants::SUCCESS;
}

// Sorts the array based on a sortOrder enum value. Using a switch statement
// to handle each of the enum values. DESCENDING and NUMBER_OCCURANCES are left
// unimplemented due to the instructions of this project.
void sortArray(constants::sortOrder so) {
	switch(so) {
		case constants::NONE :
			break;
		case constants::ASCENDING :
			for(int i = 1; i < getArraySize();) {
					if(words[i].word == "") {
						return;
					}
					for(int j = i - 1; j >= 0; j--) {
						std::string tempstr1 = words[i].word;
						std::string tempstr2 = words[j].word;
						toUpper(tempstr1);
						toUpper(tempstr2);
						if(tempstr1 < tempstr2) {
							entry temp;
							temp.word = words[j].word;
							temp.occurrances = words[j].occurrances;

							words[j].word = words[i].word;
							words[j].occurrances = words[i].occurrances;

							words[i].word = temp.word;
							words[i].occurrances = temp.occurrances;
							i = j;
						}
					}
					i++;
				}
			break;
		case constants::DESCENDING :
			break;
		case constants::NUMBER_OCCURRENCES :
			break;
	}

}
