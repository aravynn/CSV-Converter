#pragma once

// the converter class runs the actual conversion process for everything. 

#include <string>		// for data strings
#include <vector>		// for holding separated data.  
#include <iostream>		// for console stuff.
#include <fstream>		// fstream for file i/o
#include <sstream>		// stringstream for file read.
#include <algorithm>	// for find_if

#include "userInput.h"	// for easy access to later data. 
#include "structs.h"

class converter
{
private:
	userInput *m_u; // file reference to u.
	std::vector<csvLine> m_csv; // broken down data of the CSV, as gotten from the appropriate file. 
	std::vector<std::string> m_html; // broken down data of the HTML file, as gotten from the appropriate file. 

	std::string getFileContents(std::string &fileName, std::string lineBreak = "\n"); // get and return the contents of a file as a string for later parsing. 
	bool writeFileContents(std::string fileName, std::stringstream &data); // write the contents of a file. 

	void csvBreakdown(); // break apart the csv to struct files. 
	std::string updateSizeOptions(std::string &source, int denominator);
	std::string getSmallestFraction(int numerator, int denominator);

	void htmlBreakdown(); // break apart html like the CSV. 

	bool isInSection(int csvLine, int htmlLine); // check line n of csv if SAP is in section. 
	
	void generateCSVs(int maxLines = 500); // generate all of the lines into multiple CSV's
	std::vector<std::string> extractString(std::string& base, std::string start, std::string end);
public:
	converter(userInput* ui);

	std::vector<std::string> explode(const std::string& delimiter, const std::string& str); // explode function as per zedwood. looks solid enough. 

};

