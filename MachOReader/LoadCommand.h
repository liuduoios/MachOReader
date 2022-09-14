//
//  LoadCommand.hpp
//  MachOReader
//
//  Created by liuduo on 2022/9/14.
//

#ifndef LoadCommand_hpp
#define LoadCommand_hpp

#include <cstdint>

namespace MachOReader {

class LoadCommand {
public:
    uint32_t type;
    uint32_t size;
    
    LoadCommand(uint32_t type, uint32_t size);
};

} // namespace MachOReader

#endif /* LoadCommand_hpp */
