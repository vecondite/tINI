# tINI
A simple and tiny C++ INI library
> Example
```cpp
#include <iostream>
#include "./../src/tINI.h"

int main(){
    tINI::IniData iniData = tINI::read("./test.ini");
    std::cout << iniData["test"]["test"] << "\n";
}
```
