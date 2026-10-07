#ifndef TINI_H
#define TINI_H

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

namespace tINI{
    class Ini{
        public:
            using IniData = std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

            std::string trim(std::string str, std::string ttrim){
                size_t start = str.find_first_not_of(ttrim);
                if(start == std::string::npos) return str;
                size_t end = str.find_last_not_of(ttrim);
                return str.substr(start, end-start+1);
            }

            std::vector<std::string> split(std::string str, std::string del){
                std::vector<std::string> out;
                size_t start = 0;
                size_t end = 0;
                while((end = str.find_first_of(del, start))!=std::string::npos){
                    out.push_back(str.substr(start, end-start));
                    start = end+1;
                }
                if(start < str.length()) out.push_back(str.substr(start));
                return out;
            }

            IniData read(std::string file){
                IniData iniData;
                std::string spaces = " \t\n\r";
                std::ifstream iniFile;
                iniFile.open(file);
                if(iniFile.is_open()){
                    std::string currentSect;
                    std::string line;
                    while(std::getline(iniFile, line)){
                        line = split(line, ";#")[0];
                        if(line=="") continue;
                        line = trim(line, spaces);
                        if(line.front() == '[' && line.back() == ']'){
                            currentSect = trim(line, "[]");
                        }else{
                            std::vector<std::string> lineVals = split(trim(line,  spaces), "=");
                            std::string key = trim(lineVals[0], spaces);
                            std::string val = trim(lineVals[1], spaces);
                            iniData[currentSect][key] = val;
                        }
                    }

                    iniFile.close();
                    return iniData;
                }else{
                    std::cerr << "failed to open ini file!";
                    return IniData{};
                }
            }
    };
}

#endif
