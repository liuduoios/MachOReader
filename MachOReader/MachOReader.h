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

namespace MachOReader {

class MachOReader {
    std::string filePath;
public:
    std::string architecture;
    std::string cputype;
    std::string cpusubtype;
    
    MachOReader(std::string filePath);
    ~MachOReader();
private:
    void handle64Arch();
    void printCPUType(struct mach_header_64 header);
private:
    std::ifstream infile;
};

} // namespace MachOReader
