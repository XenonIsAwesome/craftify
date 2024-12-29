#ifndef BLOCKS_DB_H
#define BLOCKS_DB_H

#include <vector>
#include "baked_block.h"

class BlocksDB {
public:
    static BlocksDB *getInstance();

    BakedBlock &at(int index);
    size_t size() {return blocks.size();};

private:
    BlocksDB();

    void loadBlocksFromFile(const std::string& file_path);

    std::vector<BakedBlock> blocks;
    static BlocksDB *instance;
    
};

#endif // BLOCKS_DB_H