#include "blocks_db.h"
#include "nlohmann/json.hpp"
#include <fstream>


static BlocksDB& BlocksDB::getInstance() {
    if (instance == nullptr) instance = BlocksDB();
    return instance;
}


BakedBlock &BlocksDB::operator[] (int index) {
    return &blocks[index];
}


void BlocksDB::loadBlocksFromFile(const std::string& file_path) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    nlohmann::json json_data;
    file >> json_data;

    for (const auto& item : json_data) {
        BakedBlock block(item);
        blocks.push_back(block);
    }
}


BlocksDB::BlocksDB() {
    loadBlocksFromFile("static/baked_blocks.json");
}

