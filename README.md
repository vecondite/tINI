# tINI
A simple and tiny C++ INI library
> Example
```cpp
#include <iostream>
#include "./../src/tINI.h"

int main(){
    tINI::IniData iniData = tINI::read("./test.ini");
    std::cout << iniData["advanced"]["advancedTest4"] << "\n";
    tINI::IniData writeData;
    writeData["test"]["test1"] = "Hello";
    writeData["test"]["test2"] = "World";
    writeData["test2"]["test1"] = "Foo";
    writeData["test2"]["test2"] = "Bar";

    tINI::generate("./generated.ini", writeData);
}
```
