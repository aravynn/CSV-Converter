#include "userInput.h"

userInput::userInput() {
    // all data is user-fed for this. 

    while (!getUserInput()) {} // blank test to check for data. 

}
bool userInput::getUserInput() {

    std::cout << "Please enter the CSV file name for the upload information. it should be formatted as: Part Number, Category, Search Part#, Price \n";
    std::cout << "Category as Base/Sub/Final etc. Only mark the FIRST product in a series, the rest will be assumed as part of the same product page until a new one is declared. \n";

    std::getline(std::cin, m_partNumberData);

    // remove the quotations of moving a file to the terminal window.
    m_partNumberData.erase(0, 1);
    m_partNumberData.erase(m_partNumberData.size() - 1);

    std::cout << "\nPlease enter the HTML data file to be parsed into the final files. Separate sections by <hr />,"
        << " all tables will be entered into the main description, no specifications allowed.\n";

    std::getline(std::cin, m_fileData);

    m_fileData.erase(0, 1);
    m_fileData.erase(m_fileData.size() - 1);

    std::cout << "\nWhere should the data be saved? \n";

    std::getline(std::cin, m_outputFolder);

    m_outputFolder.erase(0, 1);
    m_outputFolder.erase(m_outputFolder.size() - 1);

    // confirmation of locations.
    std::cout << "\nUsing Files:\n CSV: " << m_partNumberData << "\n HTML: " << m_fileData << "\n Output to: " << m_outputFolder << "\n\n";

    // user must approve with y or Y
    char approved{ 'n' }; // defaults to NO.
    std::cout << "Please confirm these locations are correct: (y or n) ";
    std::cin >> approved;
    ignoreLine();

    std::cout << "\n\n Do the part numbers need to be broken down? ";
    char doUpdateParts{ 'n' };

    std::cin >> doUpdateParts;
    ignoreLine();
    std::cout << "\n\n";

    if (doUpdateParts == 'y' || doUpdateParts == 'Y') {
        m_breakdownSizes = true;
    }
    else {
        m_breakdownSizes = false;
    }

    if (approved == 'y' || approved == 'Y') {
        return true;
    }

    // in the case of a false return, we'll need to retry the data.
    return false;
}

void userInput::ignoreLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.clear();
}

// generic output functions. (one way data entry.)
std::string userInput::getPartNumberData() {
	return m_partNumberData;
}
std::string userInput::getFileData() {
	return m_fileData;
}
std::string userInput::getOutputLocation(){
	return m_outputFolder;
}

bool userInput::getSizeChange() {
    return m_breakdownSizes;
}