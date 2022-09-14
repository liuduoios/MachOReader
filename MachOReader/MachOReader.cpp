//
//  MachOReader.cpp
//  MachOReader
//
//  Created by liuduo on 2022/9/6.
//

#include "MachOReader.h"
#include <mach-o/fat.h>
#include "nlist.h"
#include <iostream>

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
    delete symbolTable;
    
}

void MachOReader::handle64Arch() {
    infile.seekg(0);
    
    struct mach_header_64 header;
    infile.read(reinterpret_cast<char *>(&header), sizeof(struct mach_header_64));
    this->header = header;
    
    printCPUType(header);
    
    // 打印文件类型
    printFileType(header);
    
    printLoadCommandList();
    
    infile.close();
    printf("文件已关闭\n");
    
    symbolTable->printAllSymbols();
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

void MachOReader::printLoadCommandList() {
    std::cout << "\n-----------------------";
    printf("\n共有 %d 个加载命令\n", header.ncmds);
    
//    std::streampos loadCommandPos = infile.tellg();
    
    for (int i = 0; i < header.ncmds; ++i) {
        // 读取头部的 load_command 结构，读取完以后需要回退文件指针
        std::streampos pos = infile.tellg();
        struct load_command lc;
        infile.read(reinterpret_cast<char *>(&lc), sizeof(struct load_command));
        infile.seekg(pos);
        
        // 根据 load_command 的类型来读取成对应的结构体
        if (lc.cmd == LC_SEGMENT) {
            struct segment_command command;
            infile.read(reinterpret_cast<char *>(&command), lc.cmdsize);
            printf("LC_SEGMENT\n");
        } else if (lc.cmd == LC_SEGMENT_64) {
            struct segment_command_64 command;
            infile.read(reinterpret_cast<char *>(&command), sizeof(struct segment_command_64));
            printf("segment_command_64.segname = %s\n", command.segname);
            
            // 如果存在 section，则需要读取 section
            for (int j = 0; j < command.nsects; ++j) {
                struct section_64 section;
                infile.read(reinterpret_cast<char *>(&section), sizeof(struct section_64));
                printf("\tsection: %s\n", section.sectname);
            }
        } else if (lc.cmd == LC_DYLD_INFO_ONLY) { // 仅包含压缩后的 dyld 信息
            struct dyld_info_command dic;
            infile.read(reinterpret_cast<char *>(&dic), lc.cmdsize);
            printf("LC_DYLD_INFO_ONLY\n");
        } else if (lc.cmd == LC_SYMTAB) { // 符号表
            struct symtab_command command;
            infile.read(reinterpret_cast<char *>(&command), lc.cmdsize);
            printf("LC_SYMTAB 有 %d 个符号\n", command.nsyms);
//            struct symtab_command {
//                uint32_t    cmd;        /* LC_SYMTAB */
//                uint32_t    cmdsize;    /* sizeof(struct symtab_command) */
//                uint32_t    symoff;     /* 符号表偏移量 */
//                uint32_t    nsyms;      /* 符号表 entries 的数量 */
//                uint32_t    stroff;     /* 字符串表的偏移量 */
//                uint32_t    strsize;    /* 字符串表的大小（字节） */
//            };
            printf("\t符号表地址偏移量：%x\n", command.symoff);
            printf("\t字符串表地址偏移量：%x\n", command.stroff);
            
            symbolTable = new SymbolTable(command, filePath);
        } else if (lc.cmd == LC_DYSYMTAB) {
            struct dysymtab_command stc;
            infile.read(reinterpret_cast<char *>(&stc), lc.cmdsize);
            printf("LC_DYSYMTAB\n");
        } else if (lc.cmd == LC_ID_DYLINKER
                   || lc.cmd == LC_LOAD_DYLINKER
                   || lc.cmd == LC_DYLD_ENVIRONMENT) {
            struct dylinker_command stc;
//            printf("%lu", sizeof(struct dylinker_command));
            infile.read(reinterpret_cast<char *>(&stc), 16); // sizeof(struct dylinker_command) == 12
            printf("LC_LOAD_DYLINKER\n");
        } else if (lc.cmd == LC_UUID) {
            struct uuid_command stc;
            infile.read(reinterpret_cast<char *>(&stc), sizeof(struct uuid_command));
            printf("LC_UUID\n");
        } else if (lc.cmd == LC_SOURCE_VERSION) {
            struct source_version_command svc;
            infile.read(reinterpret_cast<char *>(&svc), sizeof(struct source_version_command));
            printf("LC_SOURCE_VERSION\n");
        } else if (lc.cmd == LC_MAIN) {
            struct entry_point_command command;
            infile.read(reinterpret_cast<char *>(&command), sizeof(struct entry_point_command));
            printf("LC_MAIN\n");
        } else if (lc.cmd == LC_LOAD_DYLIB) { // 加载一个动态链接的共享库
            struct dylib_module_64 dylibcmd;
            infile.read(reinterpret_cast<char *>(&dylibcmd), sizeof(struct dylib_module_64));
            printf("LC_LOAD_DYLIB\n");
        } else if (lc.cmd == LC_FUNCTION_STARTS) { // 压缩了的函数起始地址表
            struct linkedit_data_command command;
            infile.read(reinterpret_cast<char *>(&command), sizeof(struct linkedit_data_command));
            printf("LC_FUNCTION_STARTS\n");
        } else if (lc.cmd == LC_DATA_IN_CODE) {
            struct linkedit_data_command command;
            infile.read(reinterpret_cast<char *>(&command), sizeof(struct linkedit_data_command));
            printf("LC_DATA_IN_CODE\n");
        } else if (lc.cmd == LC_CODE_SIGNATURE) {
            struct linkedit_data_command command;
            infile.read(reinterpret_cast<char *>(&command), sizeof(struct linkedit_data_command));
            printf("LC_CODE_SIGNATURE\n");
        } else if (lc.cmd == LC_PREBOUND_DYLIB) {
            struct prebound_dylib_command command;
            infile.read(reinterpret_cast<char *>(&command), sizeof(struct prebound_dylib_command));
            printf("LC_PREBOUND_DYLIB\n");
        } else if (lc.cmd == LC_LAZY_LOAD_DYLIB) {
            struct dylib_command command;
            infile.read(reinterpret_cast<char *>(&command), sizeof(struct dylib_command));
            printf("LC_LAZY_LOAD_DYLIB\n");
        } else if (lc.cmd == LC_LOAD_DYLIB
                   || lc.cmd == LC_ID_DYLIB
                   || lc.cmd == LC_LOAD_WEAK_DYLIB) {
            struct dylib_command command;
            infile.read(reinterpret_cast<char *>(&command), sizeof(struct dylib_command));
            printf("LC_LOAD_DYLIB\n");
        } else {
            // 不支持的 cmd 应该怎么处理？
            int64_t command;
            infile.read(reinterpret_cast<char *>(&command), 16);
            printf("lc.cmd = %x\n", lc.cmd);
        }
    }
}

} // namespace MachOReader
