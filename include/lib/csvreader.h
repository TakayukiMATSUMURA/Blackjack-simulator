#pragma once

#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

class CSVReader {
public:
    CSVReader(std::string fileName) : _fileName(fileName) {
        std::ifstream ifs(fileName);

        std::string str;
        std::vector<std::string> lows;
        while(getline(ifs,str)) {
            std::string token;
            std::istringstream stream(str);

            int lowIndex = 0;
            std::string column;
            while(getline(stream,token,';')){
                if(lowIndex == 0) {
                    column = token;
                }
                if(column == "") {
                    lows.push_back(token);
                }

                _contents[column][lows[lowIndex]] = token;

                lowIndex++;
            }
        }
    }

    virtual ~CSVReader() {
    }

    std::string getContent(std::string column, std::string low) {
        return _contents[column][low];
    }

private:
    std::string _fileName;
    std::map<std::string, std::map<std::string, std::string>> _contents;
};
