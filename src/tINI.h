/*
MIT License

Copyright (c) 2026 ᴠ ᴇ ᴄ ᴏ ɴ ᴅ ɪ ᴛ ᴇ

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef TINI_H
#define TINI_H

#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

namespace tINI{
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
        if(start <= str.length()) out.push_back(str.substr(start));
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
                line = trim(line, spaces);
                if(line=="") continue;
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
}

#endif
