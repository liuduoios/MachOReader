//
//  MachOReader.cpp
//  MachOReader
//
//  Created by liuduo on 2022/9/6.
//

#include "MachOReader.h"
#include <mach-o/fat.h>

namespace MachOReader {

void printFileType(struct mach_header_64 header);
void printLoadCommandType(struct load_command lc);

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
    
    // 打印文件类型
    printFileType(header);
    
    printf("共有 %d 个加载命令\n", header.ncmds);
    
    for (int i = 0; i < header.ncmds; ++i) {
        // 读取头部的 load_command 结构，读取完以后需要回退文件指针
        std::streampos pos = infile.tellg();
        struct load_command lc;
        infile.read(reinterpret_cast<char *>(&lc), sizeof(struct load_command));
//        printLoadCommandType(lc);
        infile.seekg(pos);
        
        // 根据 load_command 的类型来读取成对应的结构体
        if (lc.cmd == LC_SEGMENT_64) {
            struct segment_command_64 sc64;
            infile.read(reinterpret_cast<char *>(&sc64), sizeof(struct segment_command_64));
            printf("segment_command_64.segname = %s\n", sc64.segname);
            
            // 如果存在 section，则需要读取 section
            for (int j = 0; j < sc64.nsects; ++j) {
                struct section_64 s64;
                infile.read(reinterpret_cast<char *>(&s64), sizeof(struct section_64));
            }
        } else if (lc.cmd == LC_DYLD_INFO_ONLY) {
            struct dyld_info_command dic;
            infile.read(reinterpret_cast<char *>(&dic), sizeof(struct dyld_info_command));
            printf("LC_DYLD_INFO_ONLY\n");
        } else if (lc.cmd == LC_SYMTAB) {
            struct symtab_command stc;
            infile.read(reinterpret_cast<char *>(&stc), sizeof(struct symtab_command));
            printf("LC_SYMTAB\n");
        } else if (lc.cmd == LC_DYSYMTAB) {
            struct dysymtab_command stc;
            infile.read(reinterpret_cast<char *>(&stc), sizeof(struct dysymtab_command));
            printf("LC_DYSYMTAB\n");
        } else if (lc.cmd == LC_LOAD_DYLINKER) {
            struct dylinker_command stc;
            infile.read(reinterpret_cast<char *>(&stc), sizeof(struct dylinker_command));
            printf("LC_LOAD_DYLINKER\n");
        } else if (lc.cmd == LC_UUID) {
            struct uuid_command stc;
            infile.read(reinterpret_cast<char *>(&stc), sizeof(struct uuid_command));
            printf("LC_UUID\n");
        }
    }
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

void printFileType(struct mach_header_64 header) {
    switch (header.filetype) {
        case MH_OBJECT:
            printf("文件类型：目标文件\n");
            break;
        case MH_EXECUTE:
            printf("文件类型：可执行文件\n");
            break;
        default:
            printf("文件类型：未处理的文件类型\n");
            break;
    }
}

void printLoadCommandType(struct load_command lc) {
    switch (lc.cmd) {
        case LC_SEGMENT:
            printf("加载命令类型：segment\n");
            break;
        case LC_SYMTAB:
            printf("加载命令类型：符号表\n");
            break;
        case LC_SEGMENT_64:
            printf("加载命令类型：segment_64\n");
            break;
        default:
            printf("加载命令类型：其它未处理类型\n");
            break;
    }
}

} // namespace MachOReader
