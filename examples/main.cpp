#include <iostream>
#include "./../src/tINI.h"

int main(){
    tINI::IniData iniData = tINI::read("./test.ini");
    std::cout << iniData["advanced"]["advancedTest4"] << "\n";
}
