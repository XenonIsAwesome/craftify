#include "blocks_db.h"
#include <fstream>

BlocksDB *BlocksDB::instance = nullptr;


BlocksDB *BlocksDB::getInstance() {
    if (instance == nullptr) instance = new BlocksDB();
    return instance;
}


BakedBlock &BlocksDB::at(size_t index) {
    return blocks[index];
}


void BlocksDB::loadBlocksFromFile(const std::string& file_path) {
    std::ifstream ifs(file_path);
    nlohmann::json json_data = json::parse(ifs);

    for (const auto& item : json_data) {
        if (item.is_object()) { // Check if the item is an object
            BakedBlock block(item);
            blocks.push_back(block);
        } else {
            std::cerr << "Invalid block data in JSON: " << item << std::endl;
        }
    }
}


BlocksDB::BlocksDB() {
    loadBlocksFromFile("blocks_db.json");
}

