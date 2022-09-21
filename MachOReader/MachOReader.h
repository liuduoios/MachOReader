//
//  MachOReader.hpp
//  MachOReader
//
//  Created by liuduo on 2022/9/6.
//

#pragma once

#include <string>
#include <fstream>
#include "loader.h"
#include "SymbolTable.h"

namespace MachOReader {

class MachOReader {
    std::string filePath;
    std::string architecture;
    std::string cputype;
    std::string cpusubtype;
public:
    MachOReader(std::string filePath);
    ~MachOReader();
private:
    void handleFatArch();
    
    void handle32Arch(uint32_t offset);
    void printHeaderInfo(struct mach_header header);
    
    void handle64Arch(uint32_t offset);
    void printHeaderInfo(struct mach_header_64 header);
    
    void printLoadCommandList();
private:
    std::ifstream infile;
    struct mach_header_64 header;
    uint32_t currentArchOffset;
    SymbolTable *symbolTable = nullptr;
};

} // namespace MachOReader
