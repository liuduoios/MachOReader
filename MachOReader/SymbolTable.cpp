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

SymbolTable::SymbolTable(struct symtab_command command, std::string filePath): command(command), filePath(filePath) {
    infile.open(filePath);
}

SymbolTable::~SymbolTable() {
    infile.close();
}

void SymbolTable::printAllSymbols() {
    // 保存文件指针位置
    std::streampos pos = infile.tellg();
    // 找到字符串表的位置
    infile.seekg(command.stroff, std::ios_base::beg);
    std::streampos stringTablePos = infile.tellg(); // 保存字符串表的起始位置
    for (int i = 0; i < command.strsize; ++i) {
        
    }
    
    // 找到符号表的地址
    infile.seekg(command.symoff, std::ios_base::beg);
//    std::cout << "当前文件指针位置：" << std::hex << infile.tellg() << std::endl;
    
    
    // 得到 nlist 数组
    struct nlist_64 *nlists = (struct nlist_64 *)malloc(sizeof(struct nlist_64) * command.nsyms);
    std::cout << "\n-----------------------";
    std::cout << "\n共有 " << command.nsyms << " 个符号" << std::endl;
    
    for (int i = 0; i < command.nsyms; ++i) {
        struct nlist_64 nlist;
        infile.read(reinterpret_cast<char *>(&nlist), sizeof(struct nlist_64));
//                printf("unlist.n_un.n_strx：%u\n", nlist.n_un.n_strx);
        nlists[i] = nlist;
    }
    
    std::streampos pos2 = infile.tellg();
    for (int i = 0; i < command.nsyms - 1; ++i) {
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
    infile.seekg(pos2);
    
    free(nlists);
    // 还原文件指针位置
    infile.seekg(pos);
}


}
