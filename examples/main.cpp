#include <iostream>
#include "./../src/tINI.h"

int main(){
    tINI::IniData iniData = tINI::read("./test.ini");
    std::cout << iniData["test2"]["test1"] << "\n";

    tINI::IniData writeData;
    writeData["test"]["test2"] = "Hello";
    writeData["test"]["test1"] = "World";
    writeData["test2"]["test1"] = "Foo";
    writeData["test2"]["test2"] = "Bar";
    writeData["test4"]["1"] = "";
    tINI::generate("./generated.ini", writeData);
}
