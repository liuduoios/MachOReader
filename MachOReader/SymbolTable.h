//
//  SymbolTable.hpp
//  MachOReader
//
//  Created by liuduo on 2022/9/14.
//

#pragma once

#include <stdio.h>
#include "loader.h"
#include <fstream>

namespace MachOReader {

class SymbolTable {
    struct symtab_command command;
    std::ifstream infile;
    std::streampos symbolTablePos;
    std::string filePath;
public:
    SymbolTable(struct symtab_command command, std::string filePath);
    ~SymbolTable();
    void printAllSymbols();
};

} // namespace MachOReader
