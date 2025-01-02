#ifndef BLOCKS_DB_H
#define BLOCKS_DB_H

#include <vector>
#include "baked_block.h"

/**
 * @class BlocksDB
 * @brief Singleton class to manage a database of BakedBlock objects.
 */
class BlocksDB {
public:
    /**
     * @brief Get the singleton instance of BlocksDB.
     * @return Pointer to the singleton instance.
     */
    static BlocksDB *getInstance();

    /**
     * @brief Get a reference to the BakedBlock at the specified index.
     * @param index Index of the block.
     * @return Reference to the BakedBlock.
     */
    BakedBlock &at(size_t index);

    /**
     * @brief Get the number of blocks in the database.
     * @return Number of blocks.
     */
    size_t size() {return blocks.size();};

private:
    /**
     * @brief Private constructor to prevent direct instantiation.
     */
    BlocksDB();

    /**
     * @brief Load blocks from a JSON file.
     * @param file_path Path to the JSON file.
     */
    void loadBlocksFromFile(const std::string& file_path);

    std::vector<BakedBlock> blocks; ///< Vector of BakedBlock objects.
    static BlocksDB *instance; ///< Singleton instance.
};

#endif // BLOCKS_DB_H