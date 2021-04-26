#pragma once

#include <string>

// CSV lines, as RAW. 
struct csvLine {
	std::string Part; 
	std::string SAP;
	std::string Size;
	std::string Price;
	std::string Cat;
	int htmlID{ -1 }; // internal storage for later conversion. 
	int Alt{ -1 }; // for if a variation exists more than once.  

	csvLine(std::string partnumber, std::string size, std::string category, std::string sapnumber, std::string listprice) :
		Part{ partnumber }, SAP{ sapnumber }, Price{ listprice }, Cat{ category }, Size{ size } {
		// nothing is actually required here.
	}
};

