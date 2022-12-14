//
//  main.cpp
//  MachOReader
//
//  Created by liuduo on 2022/9/6.
//

#include "MachOReader.h"

int main(int argc, char *argv[]) {
    const char *path;
    
    if (argc == 1) {
        path = argv[0];
    } else {
        path = argv[1];
    }
    
    MachOReader::MachOReader reader(path);
    
    return 0;
}
