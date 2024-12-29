#ifndef BAKED_BLOCK_H
#define BAKED_BLOCK_H

#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "nlohmann/json.hpp"

struct BakedBlock {
    public:
        BakedBlock(nlohmann::json block_json);

        BakedBlock(const BakedBlock& block);

        cv::Mat loadTexture();

        std::string name;
        std::string texture_name;
        std::string game_id;
        std::string game_id_13;
        std::string block_id;
        std::string data_id;
        bool luminance;
        bool transparency;
        bool falling;
        bool redstone;
        bool survival;
        std::string version;
        size_t index;
        uint8_t red;
        uint8_t green;
        uint8_t blue;
        uint8_t alpha;
        uint8_t h;
        uint8_t s;
        uint8_t l;

        size_t deviation = 100e6;
};

#endif // BAKED_BLOCK_H