#include "converter.h"

converter::converter(userInput* ui) : m_u{ ui } {

    // proof this actually transferred.
    //std::cout << m_u->getPartNumberData();

    //writeFileContents("test", "Hello World! \n This is some data.");

    //std::string data = getFileContents(m_u->getPartNumberData());
    //std::vector<std::string> dataBreaks = explode("\n", data);
    //std::cout << dataBreaks.at(5) << " Total Count: " << dataBreaks.size() << '\n';

    // actual code starts here
    // step one: get and explode the CSV.
    std::cout << "\nbreaking down csv... ";
    csvBreakdown();
    std::cout << "complete\n";

    // step two: get and explode the html
    std::cout << "\nbreaking down html... ";
    htmlBreakdown();
    std::cout << "complete\n";

    std::cout << "\nTotal CSV Lines: " << m_csv.size() << " Total html sections: " << m_html.size() << '\n';

    // that was easy! 
    // step 3: the hard part. Go line by line, and find and create pairings of CSV lines and html data.
  //  int lastHTML = -1;
    int found = 0;

    for (size_t i{ 0 }; i < m_csv.size(); ++i) {
        // for each CSV line check the HTML line. 
    //    if (m_csv.at(i).Cat == "") {
            // this is part of the previous section 
    //       m_csv.at(i).htmlID = lastHTML;
    //    }
     //   else {
     //       lastHTML = -1;
            for (size_t q{ 0 }; q < m_html.size(); ++q) {
                // for each html element, check this line. 
                if (isInSection(i, q)) {
                    m_csv.at(i).htmlID = q; // set the HTML entity.
     //               lastHTML = q;
                    //std::cout << "Found " << m_csv.at(i).SAP << " in " << m_html.at(q) << "\n\n";
                    ++found;
                    break; // this section is correct.
                }

            }
     //   }
    }

    /*for (csvLine l : m_csv) {
        // temporary, I just want to see the data. 
        std::cout << l.Part << ' ' << l.Cat << ' ' << l.SAP << ' ' << l.Price << ' ' << l.Size << ' ' << l.htmlID << '\n';
    }*/

    std::cout << "Found " << found << "/" << m_csv.size() << " Lines total \n";

    // step 4: create CSV's. All sections are now matched to the related csv lines. We'll need to convert to csv format. 
    generateCSVs(5000); // increase total lines, we only need one file now. 


    
}

std::vector<std::string> converter::explode(const std::string& delimiter, const std::string& str)
{
    std::vector<std::string> arr;

    int strleng = str.length();
    int delleng = delimiter.length();
    if (delleng == 0)
        return arr;//no change

    int i = 0;
    int k = 0;
    while (i < strleng)
    {
        int j = 0;
        while (i + j < strleng && j < delleng && str[i + j] == delimiter[j])
            j++;
        if (j == delleng)//found delimiter
        {
            arr.push_back(str.substr(k, i - k));
            i += delleng;
            k = i;
        }
        else
        {
            i++;
        }
    }
    arr.push_back(str.substr(k, i - k));
    return arr;
}

std::string converter::getFileContents(std::string &fileName, std::string lineBreak) {
    
    std::ifstream readFile{ fileName }; // read the file. 

    if (!readFile) {
        std::cout << "ERROR: could not open file " << fileName << '\n';
        return std::string(); // return an  empty string.
    }
    std::stringstream ss;
    while (readFile) {
        std::string out; // temp var stores the string

        std::getline(readFile, out); // get the line to the next \n

        ss << out << lineBreak; // write the full data to the string. Linebreak gives the option to change how files are broken up. 
    }

    return ss.str(); // returning blank until this is written. 
}

bool converter::writeFileContents(std::string fileName, std::stringstream &data) {

    // start by creating the file for writing. 
    std::ofstream writeFile(m_u->getOutputLocation() + '\\' + fileName + ".txt"); 

    if (!writeFile) {
        std::cout << "ERROR: could not create file " << m_u->getOutputLocation() << '\\' << fileName << ".txt\n";
        return false;
    }

    // write to the file. 
    writeFile << data.str(); 

    return true; // only if the writing succeeded. 
}

void converter::csvBreakdown() {
    // take and break apart the CSV, into required parts. 

    std::string pnData = m_u->getPartNumberData();

    std::string csvBulk = getFileContents(pnData);

    std::vector<std::string> csvData = explode("\n", csvBulk);
    
    //m_csv.resize(csvData.size()); // sizeof - other file.

    //int i{ 0 }; // first string. 
    for (std::string c : csvData) {
        // take each part and create the struct to push onto m_csv
        std::vector<std::string> ci = explode("#", c); // single line. 

        // error here - out of range?
        if (ci.size() < 5) {
            std::cout << "\nLine: " << c << " was not added. \n";
        }
        else {
            if (m_u->getSizeChange()) {
                // we'll need to update the sizes to the correct fractional values. 
                ci.at(1) = updateSizeOptions(ci.at(1), 16);
            }

            std::cout << ci.at(0) << ' ' << ci.at(1) << ' ' << ci.at(2) << ' ' << ci.at(3) << ' ' << ci.at(4) << '\n';

            m_csv.push_back(csvLine(ci.at(0), ci.at(1), ci.at(2), ci.at(3), ci.at(4))); // corrected and added size.  
        }
      //  ++i;
    }

}

void converter::htmlBreakdown() {
    // break apart the HTML, much like the csv. we'll have removed the \n to make future things simpler.

    std::string fdTmp = m_u->getFileData();

    std::string htmlBulk = getFileContents(fdTmp, ""); // get the data, don't break at \n

    m_html = explode("<hr />", htmlBulk); // break apart the rules. this is already finished. 

}

bool converter::isInSection(int csvLine, int htmlLine) {
    // find sap number in html section. return true or false

    //m_csv.at(csvLine).SAP 
    //m_html.at(htmlLine)

    if (m_html.at(htmlLine).find(m_csv.at(csvLine).SAP) != std::string::npos) {
        return true;
    }

    return false; 
}

std::string converter::updateSizeOptions(std::string &source, int denominator) {
    // update the part number from format like 08-08 to 1/2" x 1/2" 
    // denominator should always be divisible by 2 only. 

    // get the singe sizes. 
    std::vector<std::string> sizes = explode("-", source);

    std::stringstream retval;

    for (std::string size : sizes) {
        // get out of invalid scenarios.
        if (size == "" || size == "Size" || size == "size") {
            continue;
        }

        if (retval.str() != "") {
            retval << " x ";
        }

        if (size.find(".") != std::string::npos) {
            // this is a decimal
            
            retval << size << '\"';
        }
        else {
            // this is a fraction. 
            // get the numerator as a string.
            int numerator = std::stoi(size);
            retval << getSmallestFraction(numerator, denominator);
            
        }
    }

    //std::cout << retval.str() << '\n';
    return retval.str();

}

std::string converter::getSmallestFraction(int numerator, int denominator) {
    // return the smallest possible logical fraction. 

    std::stringstream retval;

    int tmp = numerator;
    int whole = 0;

    if (tmp >= denominator) {
        while (tmp >= denominator) {
            ++whole;
            tmp -= denominator;
        }
    }

    numerator -= whole * denominator;

    if (numerator == 0) {
        // whole number only, we'll return that. 
        retval << whole << "\"";
    }
    else {

        // get the lowest numerator. 


        while (numerator % 2 == 0) {
            // get the smallest fraction size. 
            numerator /= 2;
            denominator /= 2;
        }

        if (whole == 0) {
            retval << numerator << "/" << denominator << "\"";
        }
        else {
            retval << whole << ' ' << numerator << "/" << denominator << "\"";
        }
    }

    return retval.str();
}

void converter::generateCSVs(int maxLines) {
    // create multiple CSV files from existing data

    // lines per CSV.
    int linesThisCSV = 1;
    int ID{ 5745787 };
    int IDStep{ 5 };
    int productID = ID;
    int files = 1;
    std::stringstream csvContent; // content for output.

    // step 1 - pull the existing html sections (how we'll actually run this) to create each listing. 
    for (size_t i{ 0 }; i < m_html.size(); ++i) {
        // step 2 - use custom find function to search the CSV for matching lines.
        std::vector<csvLine> thisSections;

        for (csvLine l : m_csv) {
            if (l.htmlID == i) {
                thisSections.push_back(l); // keep this item. 
            }
        }

        std::cout << "HTML Area #" << i << " Has " << thisSections.size() << " Valid Csv Lines \n";

        if (thisSections.size() == 0) {
            // this is an empty area, skip;
            continue;
        }


        //  take the HTML data, and break it apart into sections for images, title, and content. 
        std::string title = extractString(m_html.at(i), "<p class=\"title\">", "</p>").at(0);
        std::vector<std::string> images = extractString(m_html.at(i), "<img src=\"", "\"/>");

        // generate final content here. 
        std::vector<std::string> contentRaw = explode("<table class=\"data-table\">", m_html.at(i));
        std::stringstream content;
        for (int q{ 1 }; q < contentRaw.size(); ++q) {
            content << "<table class=\"data-table\">" << contentRaw.at(q);
        }

        // add the header for the new file.
        if (linesThisCSV == 1) {
            //std::string header = "ID,Type,SKU,Name,Published,Visibility,Description,Tax Status,In Stock?,Stock,Weight,Length,Width,Height,Price,Categories,Images,Parent,Att1Name,Att1Val,Att1Vis,Att1Glob,Att2Name,Att2Val,Att2Vis,Att2Glob";
            // new replacement header: 
            std::string header = "post_id#type#sku: sku#post_title#visibility#post_content#inventory: stock_status#inventory: stock#weight#length#width#height#price: regular_price#product_cat#images#By ID: post_parent#attribute:Size#attribute_data:Size#meta:attribute_Size#attribute:Options#attribute_data:Options#meta:attribute_Options#";
            
            csvContent << header << '\n';
        }

        linesThisCSV++;
        
        //csvContent << ++ID << ",variable,,'" << title << "',1,visible,'" << content.str() << "',taxable,1,,0.5,1,1,3,,'" << thisSections.at(0).Cat << "','";
        // replacement Content
        ID += IDStep; // step by X to avoid duplication issues in wordpress. Man I hate wordpress.
        csvContent << ID << "#variable##" << title << "#1#" << content.str() << "#1#100#0.5#1#1#3##" << thisSections.at(0).Cat << "#";
        
        productID = ID;

        bool first{ true };
        for (std::string i : images) {
            if (first) {
                csvContent << i;
            }
            else {
                csvContent << ", " << i;
            }
        }
 
        //csvContent << "',,Size,'";
        csvContent << "##";

        std::vector<std::string> variations;
        bool needsAtt2{ false };
        first = true;
        std::stringstream varString;
        for (csvLine &l : thisSections) {

            auto find = std::find(variations.begin(), variations.end(), l.Size);

            if (find != std::end(variations)) {
                l.Alt = 1;
                needsAtt2 = true; // we'll need to set the next section. 
            }
            else {
                if (first) {
                    first = false;
                    varString << l.Size;
                }
                else {
                    varString << " | " << l.Size;
                }
            }

            variations.push_back(l.Size);
        }

        //csvContent << varString.str() << "',1,0,";
        csvContent << varString.str() << "#0|1|1##";

        if (needsAtt2) {
            //csvContent << "Alternate,'FirstVal, SecondVal',1,0\n";
            csvContent << "FirstVal | SecondVal#0|1|1#\n";
        }
        else {
            //csvContent << ",,0,0\n";
            csvContent << "#0|0|0#\n";
        }

        // now that we have a section of iterations, we'll need too take this data and import it into the CSV format. 
        for (csvLine l : thisSections) {
            //    std::cout << l.Part << ' ' << l.Cat << ' ' << l.SAP << ' ' << l.Price << ' ' << l.Size << ' ' << l.htmlID << '\n';

           // l.Price = l.Price.substr(1);
            linesThisCSV++;
            //csvContent << ++ID << ",variation," << l.Part << ",'" << title << "-" << l.Size << "',1,visible,,taxable,1,100,0.5,1,1,3," << l.Price << ",,,'" <<
            //    productID << "',Size,'" << l.Size << "',1,0";

            ID += IDStep;
            csvContent << ID << "#variation#" << l.Part << "#" << title << "-" << l.Size << "#1##1#100#0.5#1#1#3#" << l.Price << "###" << productID << "###" << l.Size << "#";


            if (needsAtt2) {
                if (l.Alt == -1) {
                    //csvContent << ",Alternate,FirstVal,1,0\n";
                    csvContent << "##FirstVal\n";
                }
                else {
                    //csvContent << ",Alternate,SecondVal,1,0\n";
                    csvContent << "##SecondVal\n";
                }
            }
            else {
                //csvContent << ",,,0,0\n";
                csvContent << "###\n";
            }
        }

        // finally, check if we've added more lines than the limit. 
        if (linesThisCSV >= maxLines || i == m_html.size() - 1) {
            linesThisCSV = 1; // reset lines.

            std::stringstream filename;
            filename << "OutputFile-" << files++;
            
            std::string fileData = csvContent.str();

            writeFileContents(filename.str(), csvContent);

            // empty the field.
            csvContent.clear();
            csvContent.str(std::string());
        }

    }

    // our final task is to go through and output the incomplete lines to another CSV file, where they can be processed at a later time. 
    std::vector<csvLine> thisSections;
    for (csvLine l : m_csv) {
        if (l.htmlID == -1) {
            thisSections.push_back(l); // keep this item. 
        }
    }
    // create CSV headers. 
    csvContent << "Description,Size,MEP Brothers Category,Material,Jun 2019 List Price\n";

    // create the file to write. 
    for (csvLine l : thisSections) {
        //    std::cout << l.Part << ' ' << l.Cat << ' ' << l.SAP << ' ' << l.Price << ' ' << l.Size << ' ' << l.htmlID << '\n';

       // l.Price = l.Price.substr(1);
        linesThisCSV++;
        csvContent << l.Part << ",'" << l.Size << "','" << l.Cat << "'," << l.SAP << "," <<l.Price << '\n';

    }

    writeFileContents("NoData", csvContent);
}

std::vector<std::string> converter::extractString(std::string& base, std::string start, std::string end) {
    // this will return all strings that are appropriate for the requested one. 
    std::vector<std::string> data = explode(start, base);
    if (data.size() == 1) {
        return {"",""}; // there was nothing to return. 
    }
    std::vector<std::string> retval;

    for (size_t i{ 1 }; i < data.size(); i++) {
        retval.push_back(explode(end, data.at(i)).at(0));
    }
    
    return retval;
}