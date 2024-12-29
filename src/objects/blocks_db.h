#ifndef BLOCKS_DB_H
#define BLOCKS_DB_H

#include <vector>
#include "baked_block.h"

class BlocksDB {
public:
    static BlocksDB& getInstance();

    BakedBlock &operator[] (int index);

    size_t size() {return blocks.size();};

private:
    BlocksDB();

    void loadBlocksFromFile(const std::string& file_path);

    std::vector<BakedBlock> blocks;
    static BlocksDB &instance = nullptr;
    
};

#endif // BLOCKS_DB_H