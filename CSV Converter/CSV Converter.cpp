// CSV Converter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <limits>

#include "userInput.h"
#include "converter.h"

int main()
{
    std::cout << "CSV Generator\n";

    // application will use 2 existing CSV's and selecting data from one, produce a third and fourth CSV from the data. 
    // the output will be a final upload file, upload.csv, and a second file of failed part numbers failed.csv
    // for simplicity, all files will live in the local root. 

    userInput *u = new userInput(); // the user required data. pointer so we can pass a pointer to the converter app.

    // now that we have data, we can begin to convert the data over. 

    converter c(u);

    delete u;
}

