//
//  SwapEndian.c
//  MachOReader
//
//  Created by liuduo on 2022/9/17.
//

#include "SwapEndian.h"
#include <architecture/byte_order.h>

void swapFatHeader(struct fat_header *header) {
//    header->magic = NXSwapInt(header->magic);
    header->nfat_arch = NXSwapInt(header->nfat_arch);
}

void swapFatArch(struct fat_arch *arch) {
    arch->cputype = NXSwapInt(arch->cputype);
    arch->cpusubtype = NXSwapInt(arch->cpusubtype);
    arch->offset = NXSwapInt(arch->offset);
    arch->size = NXSwapInt(arch->size);
    arch->align = NXSwapInt(arch->align);
}

void swapFatArch64(struct fat_arch_64 *arch) {
    arch->cputype = NXSwapInt(arch->cputype);
    arch->cpusubtype = NXSwapInt(arch->cpusubtype);
    arch->offset = NXSwapInt(arch->offset);
    arch->size = NXSwapInt(arch->size);
    arch->align = NXSwapInt(arch->align);
    arch->reserved = NXSwapInt(arch->reserved);
}


