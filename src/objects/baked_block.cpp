#include "baked_block.h"
#include "../nlohmann/single_include/nlohmann/json.hpp"


BakedBlock::BakedBlock(json block_json) {
    name = block_json["name"];
    texture_name = block_json["texture_image"];
    index = block_json["index"];

    red = block_json["red"];
    green = block_json["green"];
    blue = block_json["blue"];

    deviation = 100e6;
}


BakedBlock::BakedBlock(const BakedBlock& block) {
    name = block.name;
    texture_name = block.texture_name;
    index = block.index;
    
    red = block.red;
    green = block.green;
    blue = block.blue;

    deviation = block.deviation;
}


cv::Mat BakedBlock::loadTexture() const {
    std::stringstream texture_path;
    texture_path << "assets/minecraft/textures/block/" << texture_name;
    auto block_texture =  cv::imread(texture_path.str(), cv::IMREAD_UNCHANGED);

    [[unlikely]] if (block_texture.empty()) {
        std::stringstream err;

        err << "Could not open or find the texture image!" << std::endl;
        err << "Path: " << texture_path.str();

        std::cerr << err.str() << std::endl;
        throw std::runtime_error(err.str());
    }

    return block_texture;
}

