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
#include <list>

namespace tINI{
    template<typename kT, typename vT> class ordered_map{
        using Entry = std::pair<const kT, vT>;
        using DataList = std::list<Entry>;
        using EntryIterator = typename DataList::iterator;
        using CEntryIterator = typename DataList::const_iterator;
        using LocationMap = std::unordered_map<kT, EntryIterator>;
        using LocationIterator = typename LocationMap::iterator;
        DataList data;
        LocationMap locs;
        public:
            ordered_map() = default;
            ordered_map(const ordered_map& other) : data(other.data) { for(EntryIterator it = data.begin(); it != data.end(); ++it) locs.emplace(it->first, it); }
            ordered_map& operator=(const ordered_map& other) {
                if(this!=&other){
                    ordered_map temp(other);
                    data.swap(temp.data);
                    locs.swap(temp.locs);
                }
                return *this;
            }
            ordered_map(ordered_map&&) = default;
            ordered_map& operator=(ordered_map&&) = default;

            vT& operator[](const kT& key){
                LocationIterator val = locs.find(key);
                if(val!=locs.end()){
                    return val->second->second;
                }else{
                    EntryIterator newEntry = data.emplace(data.end(), key, vT{});
                    locs.emplace(key, newEntry);
                    return newEntry->second;
                }
            }
            void erase(const kT& key){
                LocationIterator val = locs.find(key);
                if(val!=locs.end()){
                    data.erase(val->second);
                    locs.erase(val);
                }
            }
            EntryIterator begin() { return data.begin(); }
            EntryIterator end() { return data.end(); }
            CEntryIterator begin() const { return data.begin(); } 
            CEntryIterator end() const { return data.end(); }
            bool contains(const kT& key) const { return locs.find(key)!=locs.end(); }
            std::size_t size() const { return data.size(); }
            bool empty() const { return data.empty(); }
            void clear() { data.clear(); locs.clear(); }
    };

    using IniData = tINI::ordered_map<std::string, tINI::ordered_map<std::string, std::string>>;

    inline std::string trim(std::string str, std::string ttrim){
        size_t start = str.find_first_not_of(ttrim);
        if(start == std::string::npos) return "";
        size_t end = str.find_last_not_of(ttrim);
        return str.substr(start, end-start+1);
    }

    inline std::string escape(std::string str){
        std::string output;
        bool escaped = false;
        for(char c : str){
            if(escaped){
                escaped = false;
                continue;
            }else if(c=='\\'){
                escaped = true;
            }else{
                output+=c;
            }
        }
        return output;
    }

    inline std::vector<std::string> split(std::string rStr, std::string del, bool esc, bool firstOnly){
        std::vector<std::string> out;
        size_t start = 0;
        size_t end = 0;
        std::string str;
        if(esc){
            str = escape(rStr);
        }else{
            str = rStr;
        }
        while((end = str.find_first_of(del, start))!=std::string::npos){
            out.push_back(str.substr(start, end-start));
            start = end+1;
            if(firstOnly) break;
        }
        if(start <= str.length()) out.push_back(str.substr(start));
        return out;
    }

    inline IniData read(std::string file){
        IniData iniData;
        std::string spaces = " \t\n\r";
        std::ifstream iniFile;
        iniFile.open(file);
        if(iniFile.is_open()){
            std::string currentSect;
            std::string line;
            while(std::getline(iniFile, line)){
                line = split(line, ";#", false, false)[0];
                line = trim(line, spaces);
                if(line=="") continue;
                if(line.front() == '[' && line.back() == ']'){
                    currentSect = trim(line, "[]");
                }else{
                    std::vector<std::string> lineVals = split(trim(line,  spaces), "=", true, true);
                    if(lineVals.size() == 2){
                        std::string key = trim(lineVals[0], spaces);
                        std::string val = trim(lineVals[1], spaces);
                        iniData[currentSect][key] = val;
                    }
                }
            }

            iniFile.close();
            return iniData;
        }else{
            std::cerr << "failed to open ini file!";
            return IniData{};
        }
    }

    inline bool generate(std::string file, const IniData& genData){
        std::ofstream outFile(file);
        if(outFile.is_open()){
            for(const std::pair<const std::string, tINI::ordered_map<std::string, std::string>>& sect : genData){
                outFile << "[" << sect.first << "]\n";
                for(const std::pair<const std::string, std::string>& entry : sect.second){
                    outFile << entry.first << " = " << entry.second << "\n";
                }
                outFile << "\n";
            }
            outFile.close();
            return true;
        }else{
            std::cerr << "Failed to open file " << file << " for writing\n";
            return false;
        }
    }
}

#endif
