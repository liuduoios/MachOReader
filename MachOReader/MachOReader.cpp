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
#include "Architecture.h"
#include "LoadCommand.h"
#include "CPUType.h"
#include "CPUSubtype.h"
#include "FileType.h"
#include "SwapEndian.h"

namespace MachOReader {

void printFileType(struct mach_header_64 header);
void printLoadCommandType(struct load_command lc);

MachOReader::MachOReader(std::string filePath): filePath(filePath) {
    printf("%s\n", filePath.c_str());
    infile.open(filePath);
    
    // 读取 magic
    uint32_t magic;
    infile.read(reinterpret_cast<char *>(&magic), sizeof(magic));
    printf("magic: %x\n", magic);
    Architecture architecture = Architecture(magic);
    std::cout << stringOfArchitecture(architecture) << std::endl;
    
    if (isFatArch(architecture)) {
        handleFatArch();
    } else if (is64Arch(architecture)) {
        handle64Arch(0);
    } else {
        handle32Arch(0);
    }
}

MachOReader::~MachOReader() {
    if (symbolTable) {
        delete symbolTable;
    }
}

void MachOReader::handleFatArch() {
    infile.seekg(0);
    
    struct fat_header header;
    infile.read(reinterpret_cast<char *>(&header), sizeof(struct fat_header));
    
    if (header.magic == FAT_MAGIC || header.magic == FAT_CIGAM) {
        if (header.magic == FAT_CIGAM) {
            swapFatHeader(&header);
        }
        
        for (int i = 0; i < header.nfat_arch; ++i) {
            struct fat_arch arch;
            infile.read(reinterpret_cast<char *>(&arch), sizeof(struct fat_arch));
            if (header.magic == FAT_CIGAM) {
                swapFatArch(&arch);
            }
            
            std::streampos pos = infile.tellg();
            CPUType cpuType = CPUType(arch.cputype);
            if (is64Arch(cpuType)) {
                handle64Arch((uint32_t)arch.offset);
            } else {
                handle32Arch((uint32_t)arch.offset);
            }
            infile.seekg(pos);
        }
    } else if (header.magic == FAT_MAGIC_64 || header.magic == FAT_CIGAM_64) {
        for (int i = 0; i < header.nfat_arch; ++i) {
            struct fat_arch_64 arch;
            infile.read(reinterpret_cast<char *>(&arch), sizeof(struct fat_arch_64));
            if (header.magic == FAT_CIGAM_64) {
                swapFatArch64(&arch);
            }
            
            std::streampos pos = infile.tellg();
            handle64Arch((uint32_t)arch.offset);
            infile.seekg(pos);
        }
    }
}

void MachOReader::handle32Arch(uint32_t offset) {
    infile.seekg(offset);
    
    struct mach_header header;
    infile.read(reinterpret_cast<char *>(&header), sizeof(struct mach_header));
//    this->header
    
    printHeaderInfo(header);
}

void MachOReader::printHeaderInfo(struct mach_header header) {
    
}

void MachOReader::handle64Arch(uint32_t offset) {
    infile.seekg(offset);
    
    struct mach_header_64 header;
    infile.read(reinterpret_cast<char *>(&header), sizeof(struct mach_header_64));
    this->header = header;
    
    printHeaderInfo(header);
    
    printLoadCommandList();
    
    if (symbolTable) {
        symbolTable->printAllSymbols();
    }
}

void MachOReader::printHeaderInfo(struct mach_header_64 header) {
    // cputype
    CPUType cpuType = CPUType(header.cputype);
    std::cout << "cputype: " << stringOfCPUType(cpuType) << std::endl;
    
    // cpusubtype
    switch (cpuType) {
        case X86_64: {
            X86CPUSubtype subtype = X86CPUSubtype(header.cpusubtype);
            std::cout << "cpusubtype: " << stringOfX86CPUSubtype(subtype) << std::endl;
            break;
        }
        default:
            break;
    }
    
    // filetype
    FileType fileType = FileType(header.filetype);
    std::cout << "filetype: " << stringOfFileType(fileType) << std::endl;
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
        
//        if (lc.cmd <= 0 || lc.cmd > LC_FILESET_ENTRY) {
//            std::cout << "无效的命令\n";
//            continue;
//        }
        
        LoadCommandType type = LoadCommandType(lc.cmd);
        std::cout << stringOfLoadCommandType(type) << std::endl;
        
        // 根据 load_command 的类型来读取成对应的结构体
        switch (lc.cmd) {
            case LC_SEGMENT: {
                struct segment_command command;
                infile.read(reinterpret_cast<char *>(&command), sizeof(struct segment_command));
                break;
            }
            case LC_SEGMENT_64: { // segment 是一种段结构，里面可能包含多个 section
                std::streampos pos = infile.tellg();
                struct segment_command_64 command;
                infile.read(reinterpret_cast<char *>(&command), sizeof(struct segment_command_64));
                printf("segment_command_64.segname = %s\n", command.segname);
                infile.seekg(pos);
                infile.seekg(lc.cmdsize, std::ios_base::cur);
                
                // 如果存在 section，则需要读取 section
                for (int j = 0; j < command.nsects; ++j) {
                    struct section_64 section;
                    infile.read(reinterpret_cast<char *>(&section), sizeof(struct section_64));
                    printf("\tsection: %s\n", section.sectname);
                }
                break;
            }
            case LC_CODE_SIGNATURE:
            case LC_SEGMENT_SPLIT_INFO:
            case LC_FUNCTION_STARTS:    // 压缩了的函数起始地址表
            case LC_DATA_IN_CODE:
            case LC_DYLIB_CODE_SIGN_DRS:
            case LC_LINKER_OPTIMIZATION_HINT:
            case LC_DYLD_EXPORTS_TRIE:
            case LC_DYLD_CHAINED_FIXUPS: {
                std::streampos pos = infile.tellg();
                struct linkedit_data_command command;
                infile.read(reinterpret_cast<char *>(&command), sizeof(struct linkedit_data_command));
                printf("linkedit_data_command\n");
                infile.seekg(pos);
                infile.seekg(command.cmdsize, std::ios_base::cur);
                break;
            }
            case LC_ID_DYLINKER:
            case LC_LOAD_DYLINKER:
            case LC_DYLD_ENVIRONMENT: {
                std::streampos pos = infile.tellg();
                struct dylinker_command command;
    //            printf("%lu", sizeof(struct dylinker_command));
                infile.read(reinterpret_cast<char *>(&command), sizeof(struct dylinker_command)); // sizeof(struct dylinker_command) == 12
                printf("LC_LOAD_DYLINKER\n");
                infile.seekg(pos);
                infile.seekg(command.cmdsize, std::ios_base::cur);
                break;
            }
            case LC_ID_DYLIB:
            case LC_LOAD_DYLIB:
            case LC_LOAD_WEAK_DYLIB:
            case LC_REEXPORT_DYLIB: {
                std::streampos pos = infile.tellg();
                struct dylib_command command;
                infile.read(reinterpret_cast<char *>(&command), sizeof(struct dylib_command));
                printf("LC_LOAD_DYLIB\n");
                infile.seekg(pos);
                infile.seekg(command.cmdsize, std::ios_base::cur);
                break;
            }
            case LC_MAIN: {
                std::streampos pos = infile.tellg();
                struct entry_point_command command;
                infile.read(reinterpret_cast<char *>(&command), sizeof(struct entry_point_command));
                printf("LC_MAIN\n");
                infile.seekg(pos);
                infile.seekg(command.cmdsize, std::ios_base::cur);
                break;
            }
            case LC_DYLD_INFO:
            case LC_DYLD_INFO_ONLY: {
                std::streampos pos = infile.tellg();
                struct dyld_info_command dic;
                infile.read(reinterpret_cast<char *>(&dic), lc.cmdsize);
                printf("LC_DYLD_INFO_ONLY\n");
                infile.seekg(pos);
                infile.seekg(lc.cmdsize, std::ios_base::cur);
                break;
            }
            case LC_SYMTAB: {
                std::streampos pos = infile.tellg();
                struct symtab_command command;
                infile.read(reinterpret_cast<char *>(&command), lc.cmdsize);
                
                printf("LC_SYMTAB 有 %d 个符号\n", command.nsyms);
                printf("\t符号表地址偏移量：%x\n", command.symoff);
                printf("\t字符串表地址偏移量：%x\n", command.stroff);
                
                infile.seekg(pos);
                infile.seekg(command.cmdsize, std::ios_base::cur);
                
                symbolTable = new SymbolTable(command, filePath);
                break;
            }
            case LC_DYSYMTAB: {
                std::streampos pos = infile.tellg();
                struct dysymtab_command stc;
                infile.read(reinterpret_cast<char *>(&stc), lc.cmdsize);
                printf("LC_DYSYMTAB\n");
                infile.seekg(pos);
                infile.seekg(lc.cmdsize, std::ios_base::cur);
                break;
            }
            case LC_UUID: {
                std::streampos pos = infile.tellg();
                struct uuid_command stc;
                infile.read(reinterpret_cast<char *>(&stc), sizeof(struct uuid_command));
                printf("LC_UUID\n");
                infile.seekg(pos);
                infile.seekg(lc.cmdsize, std::ios_base::cur);
                break;
            }
            case LC_SOURCE_VERSION: {
                std::streampos pos = infile.tellg();
                struct source_version_command svc;
                infile.read(reinterpret_cast<char *>(&svc), sizeof(struct source_version_command));
                printf("LC_SOURCE_VERSION\n");
                infile.seekg(pos);
                infile.seekg(lc.cmdsize, std::ios_base::cur);
                break;
            }
            default: {
                std::streampos pos = infile.tellg();
                // 不支持的 cmd 应该怎么处理？
                int64_t command;
                infile.read(reinterpret_cast<char *>(&command), 16);
                printf("lc.cmd = %x\n", lc.cmd);
                infile.seekg(pos);
                infile.seekg(lc.cmdsize, std::ios_base::cur);
                break;
            }
        }
        if (lc.cmd == LC_PREBOUND_DYLIB) {
            struct prebound_dylib_command command;
            infile.read(reinterpret_cast<char *>(&command), sizeof(struct prebound_dylib_command));
            printf("LC_PREBOUND_DYLIB\n");
            infile.seekg(pos);
            infile.seekg(command.cmdsize, std::ios_base::cur);
        } else if (lc.cmd == LC_LAZY_LOAD_DYLIB) {
            struct dylib_command command;
            infile.read(reinterpret_cast<char *>(&command), sizeof(struct dylib_command));
            printf("LC_LAZY_LOAD_DYLIB\n");
            infile.seekg(pos);
            infile.seekg(command.cmdsize, std::ios_base::cur);
        }
    }
}

} // namespace MachOReader
