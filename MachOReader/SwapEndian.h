//
//  SwapEndian.h
//  MachOReader
//
//  Created by hj on 2022/9/17.
//

#ifndef SwapEndian_h
#define SwapEndian_h

#include <stdio.h>
#include <mach-o/fat.h>

void swapFatArch(struct fat_arch *arch);
void swapFatArch64(struct fat_arch_64 *arch);

#endif /* SwapEndian_h */
