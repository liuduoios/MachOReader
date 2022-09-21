//
//  SymbolTable.cpp
//  MachOReader
//
//  Created by liuduo on 2022/9/14.
//

#include "SymbolTable.h"
#include <iostream>
#include "loader.h"
#include "nlist.h"

extern char *filePath;

namespace MachOReader {

SymbolTable::SymbolTable(uint32_t archOffset, struct symtab_command command, std::string filePath): archOffset(archOffset), command(command), filePath(filePath) {
    infile.open(filePath);
    infile.seekg(archOffset);
}

SymbolTable::~SymbolTable() {
    infile.close();
}

void SymbolTable::printAllSymbols() {
    // 保存文件指针位置
    std::streampos pos = infile.tellg();
    
    // 找到字符串表的位置
    infile.seekg(archOffset);
    infile.seekg(command.stroff, std::ios_base::cur);
    
    std::streampos stringTablePos = infile.tellg(); // 保存字符串表的起始位置
    for (int i = 0; i < command.strsize; ++i) {
        
    }
    
    // 找到符号表的地址
    infile.seekg(archOffset);
    infile.seekg(command.symoff, std::ios_base::cur);
    
    // 得到 nlist 数组
    struct nlist_64 *nlists = (struct nlist_64 *)malloc(sizeof(struct nlist_64) * command.nsyms);
    std::cout << "\n共有 " << std::dec << command.nsyms << " 个符号：" << std::endl;
    
    for (int i = 0; i < command.nsyms; ++i) {
        struct nlist_64 nlist;
        infile.read(reinterpret_cast<char *>(&nlist), sizeof(struct nlist_64));
        nlists[i] = nlist;
    }
    
    for (int i = 0; i < command.nsyms; ++i) {
        // 找到符号在字符串表的偏移位置
        infile.seekg(stringTablePos);
        infile.seekg(nlists[i].n_un.n_strx, std::ios_base::cur);
        int stringLength = nlists[i + 1].n_un.n_strx - nlists[i].n_un.n_strx;
        if (stringLength > 0) {
            char byte[stringLength + 1];
            infile.read(reinterpret_cast<char *>(&byte), stringLength);
            std::string symbolString = std::string(byte);
            if (symbolString.size() > 0) {
                std::cout << std::hex << symbolString << std::endl;
            }
        }
    }
    
    free(nlists);
    // 还原文件指针位置
    infile.seekg(pos);
}

} // namespace MachOReader
