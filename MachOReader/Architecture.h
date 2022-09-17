//
//  Architecture.h
//  MachOReader
//
//  Created by liuduo on 2022/9/17.
//

#pragma once

#include <string>

enum Architecture {
    FAT_BIG_ENDIAN = 0xcafebabe,
    FAT_BIG_ENDIAN_64 = 0xcafebabf,
    FAT_LITTLE_ENDIAN = 0xbebafeca,
    FAT_LITTLE_ENDIAN_64 = 0xbfbafeca,
    
    LITTER_ENDIAN = 0xfeedface,
    LITTER_ENDIAN_64 = 0xfeedfacf,
    
    ARCH_BIG_ENDIAN = 0xcefaedfe,
    ARCH_BIG_ENDIAN_64 = 0xcffaedfe
};

std::string stringOfArchitecture(Architecture architecture) {
    switch (architecture) {
        case FAT_LITTLE_ENDIAN:
            return "胖二进制 - 32位 - 小端";
        case FAT_LITTLE_ENDIAN_64:
            return "胖二进制 - 64位 - 小端";
        case FAT_BIG_ENDIAN:
            return "胖二进制 - 32位 - 大端";
        case FAT_BIG_ENDIAN_64:
            return "胖二进制 - 64位 - 大端";
        case LITTER_ENDIAN:
            return "32位架构 - 小端";
        case LITTER_ENDIAN_64:
            return "64位架构 - 小端";
        case ARCH_BIG_ENDIAN:
            return "64位架构 - 小端";
        case ARCH_BIG_ENDIAN_64:
            return "64位架构 - 大端";
    }
}

bool isFatArch(Architecture architecture) {
    switch (architecture) {
        case FAT_LITTER_ENDIAN:
        case FAT_LITTER_ENDIAN_64:
        case FAT_BIG_ENDIAN:
        case FAT_BIG_ENDIAN_64:
            return true;
        default:
            return false;
    }
}

bool is64Arch(Architecture architecture) {
    switch (architecture) {
        case FAT_LITTER_ENDIAN_64:
        case FAT_BIG_ENDIAN_64:
        case LITTER_ENDIAN_64:
        case ARCH_BIG_ENDIAN_64:
            return true;
        default:
            return false;
    }
}

bool isBigEndian(Architecture architecture) {
    switch (architecture) {
        case FAT_BIG_ENDIAN:
        case FAT_BIG_ENDIAN_64:
        case ARCH_BIG_ENDIAN:
        case ARCH_BIG_ENDIAN_64:
            return true;
        default:
            return false;
    }
}


