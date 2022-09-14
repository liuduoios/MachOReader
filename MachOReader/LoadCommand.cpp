//
//  LoadCommand.cpp
//  MachOReader
//
//  Created by liuduo on 2022/9/14.
//

#include "LoadCommand.h"

namespace MachOReader {

LoadCommand::LoadCommand(uint32_t type, uint32_t size): type(type), size(size) {
    
}

}
