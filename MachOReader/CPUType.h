//
//  CPUType.h
//  MachOReader
//
//  Created by hj on 2022/9/17.
//

#pragma once

#include <mach/machine.h>
#include <string>

enum CPUType: cpu_type_t {
    VAX = 1,
    MC680x0 = 6,
    X86 = 7,
//    I386 = X86,
    X86_64 = (X86 | CPU_ARCH_ABI64),
    MC98000 = 9,
    HPPA = 11,
    ARM = 12,
    ARM64 = (ARM | CPU_ARCH_ABI64),
    ARM64_32 = (ARM | CPU_ARCH_ABI64_32),
    MC88000 = 13,
    SPARC = 14,
    I860 = 15,
    POWERPC = 18,
    POWERPC64 = (POWERPC | CPU_ARCH_ABI64),
};

std::string stringOfCPUType(CPUType cpuType) {
    switch (cpuType) {
        case VAX:
            return "VAX";
        case MC680x0:
            return "MC680x0";
        case X86: // I386
            return "X86";
        case X86_64:
            return "X86_64";
        case MC98000:
            return "MC98000";
        case HPPA:
            return "HPPA";
        case ARM:
            return "ARM";
        case ARM64:
            return "ARM64";
        case ARM64_32:
            return "ARM64_32";
        case MC88000:
            return "MC88000";
        case SPARC:
            return "SPARC";
        case I860:
            return "I860";
        case POWERPC:
            return "POWERPC";
        case POWERPC64:
            return "POWERPC64";
    }
}

bool is64Arch(CPUType cpuType) {
    switch (cpuType) {
        case X86_64:
        case ARM64:
            return true;
        default:
            return false;
    }
}
