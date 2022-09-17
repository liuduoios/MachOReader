//
//  FileType.h
//  MachOReader
//
//  Created by hj on 2022/9/17.
//

#pragma once

#include <string>

enum FileType {
    OBJECT = 0x1,       // 可重定位的目标文件
    EXECUTE = 0x2,      // 需要分页的可执行文件
    FVMLIB = 0x3,       // 固定的虚拟内存共享库文件
    CORE = 0x4,         // 核心文件
    PRELOAD = 0x5,      // 预加载的可执行文件
    DYLIB = 0x6,        // 动态绑定的共享库
    DYLINKER = 0x7,     // 动态链接器
    BUNDLE = 0x8,       // 动态绑定的 Bundle 文件
    DYLIB_STUB = 0x9,   // 
    DSYM = 0xa,
    KEXT_BUNDLE = 0xb,
    FILESET = 0xc,
};

std::string stringOfFileType(FileType fileType) {
    switch (fileType) {
        case OBJECT:
            return "OBJECT";
        case EXECUTE:
            return "EXECUTE";
        case FVMLIB:
            return "FVMLIB";
        case CORE:
            return "CORE";
        case PRELOAD:
            return "PRELOAD";
        case DYLIB:
            return "DYLIB";
        case DYLINKER:
            return "DYLINKER";
        case BUNDLE:
            return "BUNDLE";
        case DYLIB_STUB:
            return "DYLIB_STUB";
        case DSYM:
            return "DSYM";
        case KEXT_BUNDLE:
            return "KEXT_BUNDLE";
        case FILESET:
            return "FILESET";
    }
}
