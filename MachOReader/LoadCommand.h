//
//  LoadCommand.h
//  MachOReader
//
//  Created by liuduo on 2022/9/15.
//

#pragma once

#include <string>
#include <sstream>

typedef enum : uint32_t {
    SEGMENT = 0x1,      // 段
    SYMTAB = 0x2,       // 符号表
    SYMSEG = 0x3,       // link-edit stab 格式的符号表信息
    THREAD = 0x4,       // 线程
    UNIXTHREAD = 0x5,   // Unix 线程（包含一个栈）
    LOADFVMLIB = 0x6,   // 加载一个指定的固定虚拟内存共享库
    IDFVMLIB = 0x7,     // 固定的虚拟内存共享库身份标识
    IDENT = 0x8,        // 对象身份信息（已废弃）
    FVMFILE = 0x9,      // 固定的 VM 文件包含（内部使用）
    PREPAGE = 0xa,            // 上一页的命令（内部使用）
    DYSYMTAB = 0xb,           // 动态
    LOAD_DYLIB = 0xc,
    ID_DYLIB = 0xd,
    LOAD_DYLINKER = 0xe,
    ID_DYLINKER = 0xf,
    PREBOUND_DYLIB = 0x10,
    ROUTINES = 0x11,
    SUB_FRAMEWORK = 0x12,
    SUB_UMBRELLA = 0x13,
    SUB_CLIENT= 0x14,
    SUB_LIBRARY= 0x15,
    TWOLEVEL_HINTS = 0x16,
    PREBIND_CKSUM = 0x17,
    LOAD_WEAK_DYLIB = (0x18 | LC_REQ_DYLD),
    SEGMENT_64 = 0x19,
    ROUTINES_64 = 0x1a,
    UUID = 0x1b,
    RPATH = (0x1c | LC_REQ_DYLD),
    CODE_SIGNATURE = 0x1d,
    SEGMENT_SPLIT_INFO = 0x1e,
    REEXPORT_DYLIB = (0x1f | LC_REQ_DYLD),
    LAZY_LOAD_DYLIB = 0x20,
    ENCRYPTION_INFO = 0x21,
    DYLD_INFO = 0x22,
    DYLD_INFO_ONLY = (0x22 | LC_REQ_DYLD),
    LOAD_UPWARD_DYLIB = (0x23 | LC_REQ_DYLD),
    VERSION_MIN_MACOSX = 0x24,
    VERSION_MIN_IPHONEOS = 0x25,
    FUNCTION_STARTS = 0x26,
    DYLD_ENVIRONMENT = 0x27,
    MAIN = (0x28 | LC_REQ_DYLD),
    DATA_IN_CODE = 0x29,
    SOURCE_VERSION = 0x2a,
    DYLIB_CODE_SIGN_DRS = 0x2b,
    ENCRYPTION_INFO_64 = 0x2c,
    LINKER_OPTION = 0x2d,
    LINKER_OPTIMIZATION_HINT = 0x2e,
    VERSION_MIN_TVOC = 0x2f,
    VERSION_MIN_WATCHOS = 0x30,
    NOTE = 0x31,
    BUILD_VERSION = 0x32,
    DYLD_EXPORTS_TRIE = (0x33 | LC_REQ_DYLD),
    DYLD_CHAINED_FIXUPS = (0x34 | LC_REQ_DYLD),
    FILESET_ENTRY = (0x35 | LC_REQ_DYLD)
} LoadCommandType;

std::string stringOfLoadCommandType(LoadCommandType type) {
    switch (type) {
        case SEGMENT:
            return "LC_SEGMENT";
        case SYMTAB:
            return "LC_SYMTAB";
        case SYMSEG:
            return "LC_SYMSEG";
        case THREAD:
            return "LC_THREAD";
        case UNIXTHREAD:
            return "LC_UNIXTHREAD";
        case LOADFVMLIB:
            return "LC_LOADFVMLIB";
        case IDFVMLIB:
            return "LC_IDFVMLIB";
        case IDENT:
            return "LC_IDENT";
        case FVMFILE:
            return "LC_FVMFILE";
        case PREPAGE:
            return "LC_PREPAGE";
        case DYSYMTAB:
            return "LC_DYSYMTAB";
        case LOAD_DYLIB:
            return "LC_LOAD_DYLIB";
        case ID_DYLIB:
            return "LC_ID_DYLIB";
        case LOAD_DYLINKER:
            return "LC_LOAD_DYLINKER";
        case ID_DYLINKER:
            return "LC_ID_DYLINKER";
        case PREBOUND_DYLIB:
            return "LC_PREBOUND_DYLIB";
        case ROUTINES:
            return "LC_ROUTINES";
        case SUB_FRAMEWORK:
            return "LC_SUB_FRAMEWORK";
        case SUB_UMBRELLA:
            return "LC_SUB_UMBRELLA";
        case SUB_CLIENT:
            return "LC_SUB_CLIENT";
        case SUB_LIBRARY:
            return "LC_SUB_LIBRARY";
        case TWOLEVEL_HINTS:
            return "LC_TWOLEVEL_HINTS";
        case PREBIND_CKSUM:
            return "LC_PREBIND_CKSUM";
        case LOAD_WEAK_DYLIB:
            return "LC_LOAD_WEAK_DYLIB";
        case SEGMENT_64:
            return "LC_SEGMENT_64";
        case ROUTINES_64:
            return "LC_ROUTINES_64";
        case UUID:
            return "LC_UUID";
        case RPATH:
            return "LC_RPATH";
        case CODE_SIGNATURE:
            return "LC_CODE_SIGNATURE";
        case SEGMENT_SPLIT_INFO:
            return "LC_SEGMENT_SPLIT_INFO";
        case REEXPORT_DYLIB:
            return "LC_REEXPORT_DYLIB";
        case LAZY_LOAD_DYLIB:
            return "LC_LAZY_LOAD_DYLIB";
        case ENCRYPTION_INFO:
            return "LC_ENCRYPTION_INFO";
        case DYLD_INFO:
            return "LC_DYLD_INFO";
        case DYLD_INFO_ONLY:
            return "LC_DYLD_INFO_ONLY";
        case LOAD_UPWARD_DYLIB:
            return "LC_LOAD_UPWARD_DYLIB";
        case VERSION_MIN_MACOSX:
            return "LC_VERSION_MIN_MACOSX";
        case VERSION_MIN_IPHONEOS:
            return "LC_VERSION_MIN_IPHONEOS";
        case FUNCTION_STARTS:
            return "LC_FUNCTION_STARTS";
        case DYLD_ENVIRONMENT:
            return "LC_DYLD_ENVIRONMENT";
        case MAIN:
            return "LC_MAIN";
        case DATA_IN_CODE:
            return "LC_DATA_IN_CODE";
        case SOURCE_VERSION:
            return "LC_SOURCE_VERSION";
        case DYLIB_CODE_SIGN_DRS:
            return "LC_DYLIB_CODE_SIGN_DRS";
        case ENCRYPTION_INFO_64:
            return "LC_ENCRYPTION_INFO_64";
        case LINKER_OPTION:
            return "LC_LINKER_OPTION";
        case LINKER_OPTIMIZATION_HINT:
            return "LC_LINKER_OPTIMIZATION_HINT";
        case VERSION_MIN_TVOC:
            return "LC_VERSION_MIN_TVOC";
        case VERSION_MIN_WATCHOS:
            return "LC_VERSION_MIN_WATCHOS";
        case NOTE:
            return "LC_NOTE";
        case BUILD_VERSION:
            return "LC_BUILD_VERSION";
        case DYLD_EXPORTS_TRIE:
            return "LC_DYLD_EXPORTS_TRIE";
        case DYLD_CHAINED_FIXUPS:
            return "LC_DYLD_CHAINED_FIXUPS";
        case FILESET_ENTRY:
            return "LC_FILESET_ENTRY";
        default: {
            std::stringstream ss;
            ss << "未知的命令 type: ";
            ss << std::hex << type << '\n';
            std::string string;
            ss >> string;
            return string;
        }
    }
}
