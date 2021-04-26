#pragma once

// user input stores all data related to the user, when requested. only one such element will exist and be used at the base.
#include <iostream>
#include <string>
#include <vector>
#include <limits>

class userInput
{
private:
    std::string m_partNumberData; // part number information, used for determining what goes where
    std::string m_fileData; // dataHTML, will be used to create the actual items.
    std::string m_outputFolder; // output folder for all created text files.
    bool m_breakdownSizes; // sizes require breaking down? 1/16 for hydraulics.

    void ignoreLine(); // ignore excess input.
public:
	userInput();
    bool getUserInput();

    std::string getPartNumberData();
    std::string getFileData();
    std::string getOutputLocation();
    bool getSizeChange();
};
