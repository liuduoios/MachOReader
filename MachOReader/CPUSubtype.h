//
//  CPUSubtype.h
//  MachOReader
//
//  Created by hj on 2022/9/17.
//

#pragma once

#include <mach/machine.h>
#include <string>

enum X86CPUSubtype: cpu_subtype_t {
    X86_ALL = 3,
//    X86_64_ALL = 3,
    X86_ARCH1 = 4,
    X86_64_H = 8
};

std::string stringOfX86CPUSubtype(X86CPUSubtype subtype) {
    switch (subtype) {
        case X86_ALL:
            return "X86_ALL";
//        case X86_64_ALL:
//            return "X86_64_ALL";
        case X86_ARCH1:
            return "X86_ARCH1";
        case X86_64_H:
            return "X86_64_H";
    }
}

enum ARMCPUSubtype: cpu_subtype_t {
    ARM_ALL = 0,
    ARM_V4T = 5,
    ARM_V6 = 6,
    ARM_V5TEJ = 7,
    ARM_XSCALE = 8,
    ARM_V7 = 9,
    ARM_V7F = 10,
    ARM_V7S = 11,
    ARM_V7K = 12,
    ARM_V8 = 13,
    ARM_V6M = 14,
    ARM_V7M = 15,
    ARM_V7EM = 16,
    ARM_V8M = 17
};

enum ARM64CPUSubtype: cpu_subtype_t {
    ARM64_ALL = 0,
    ARM64_V8 = 1,
    ARM64E = 2
};
