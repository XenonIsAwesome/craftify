#include "baked_block.h"


BakedBlock::BakedBlock(nlohmann::json block_json) {
    name = block_json["name"];
    texture_name = block_json["texture_name"];
    game_id = block_json["game_id"];
    game_id_13 = block_json["game_id_13"];
    block_id = block_json["block_id"];
    data_id = block_json["data_id"];
    luminance = block_json["luminance"];
    transparency = block_json["transparency"];
    falling = block_json["falling"];
    redstone = block_json["redstone"];
    survival = block_json["survival"];
    version = block_json["version"];
    index = block_json["index"];
    red = block_json["red"];
    green = block_json["green"];
    blue = block_json["blue"];
    alpha = block_json["alpha"];
    h = block_json["h"];
    s = block_json["s"];
    l = block_json["l"];

    deviation = 100e6;
}

BakedBlock::BakedBlock(BakedBlock& block) {
    name = block.name;
    texture_name = block.texture_name;
    game_id = block.game_id;
    game_id_13 = block.game_id_13;
    block_id = block.block_id;
    data_id = block.data_id;
    luminance = block.luminance;
    transparency = block.transparency;
    falling = block.falling;
    redstone = block.redstone;
    survival = block.survival;
    version = block.version;
    index = block.index;
    red = block.red;
    green = block.green;
    blue = block.blue;
    alpha = block.alpha;
    h = block.h;
    s = block.s;
    l = block.l;

    deviation = block.deviation;
}


cv::Mat loadTexture() {
    return cv::imread("static/textures/" + texture_name, cv::IMREAD_UNCHANGED);
}

