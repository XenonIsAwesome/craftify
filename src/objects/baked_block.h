#ifndef BAKED_BLOCK_H
#define BAKED_BLOCK_H

#include <opencv2/opencv.hpp>
#include "../nlohmann/single_include/nlohmann/json.hpp"

using json = nlohmann::json;

struct BakedBlock {
    public:
        BakedBlock(json block_json);

        BakedBlock(const BakedBlock& block);

        cv::Mat loadTexture();

        std::string name;
        std::string texture_name;
        size_t index;
        uint8_t red;
        uint8_t green;
        uint8_t blue;

        size_t deviation = 100e6;
};

#endif // BAKED_BLOCK_H