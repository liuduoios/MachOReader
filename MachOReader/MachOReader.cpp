//
//  MachOReader.cpp
//  MachOReader
//
//  Created by liuduo on 2022/9/6.
//

#include "MachOReader.h"
#include <mach-o/fat.h>

namespace MachOReader {

MachOReader::MachOReader(std::string filePath): filePath(filePath) {
    printf("%s\n", filePath.c_str());
    infile.open(filePath);
    
    uint32_t magic;
    infile.read(reinterpret_cast<char *>(&magic), sizeof(magic));
    printf("magic: %x\n", magic);
    if (magic == FAT_MAGIC || magic == FAT_CIGAM) {
        printf("胖二进制\n");
    } else if (magic == MH_MAGIC || magic == MH_CIGAM) {
        printf("32位架构\n");
    } else if (magic == MH_MAGIC_64 || magic == MH_CIGAM_64) {
        printf("64位架构\n");
        handle64Arch();
    } else {
        printf("未知架构\n");
    }
}

MachOReader::~MachOReader() {
    infile.close();
    printf("文件已关闭\n");
}

void MachOReader::handle64Arch() {
    infile.seekg(0);
    
    struct mach_header_64 header;
    infile.read(reinterpret_cast<char *>(&header), sizeof(struct mach_header_64));
    printCPUType(header);
}

void MachOReader::printCPUType(struct mach_header_64 header) {
    switch (header.cputype) {
        case CPU_TYPE_ARM:
            printf("cputype: ARM\n");
            break;
        case CPU_TYPE_ARM64:
            printf("cputype: ARM64\n");
            break;
        case CPU_TYPE_X86:
            printf("cputype: X86\n");
            break;
        default:
            printf("cputype: %d\n", header.cputype);
            break;
    }
}

} // namespace MachOReader
