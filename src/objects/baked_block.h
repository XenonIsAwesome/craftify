#ifndef BAKED_BLOCK_H
#define BAKED_BLOCK_H

#include <opencv2/opencv.hpp>
#include "../nlohmann/single_include/nlohmann/json.hpp"

using json = nlohmann::json;


/**
 * @class BakedBlock
 * @brief Class representing a block with texture and color information.
 */
struct BakedBlock {
    public:
        /**
         * @brief Construct a BakedBlock from JSON data.
         * @param block_json JSON object containing block data.
         */
        explicit BakedBlock(json block_json);

        /**
         * @brief Copy constructor for BakedBlock.
         * @param block BakedBlock object to copy.
         */
        BakedBlock(const BakedBlock& block);

        /**
         * @brief Load the texture image for the block.
         * @return OpenCV matrix containing the texture image.
         */
        cv::Mat loadTexture() const;

        std::string name; ///< Name of the block.
        std::string texture_name; ///< Name of the texture image file.
        size_t index; ///< Index of the block.
        uint8_t red; ///< Red color component.
        uint8_t green; ///< Green color component.
        uint8_t blue; ///< Blue color component.

        size_t deviation = 100e6; ///< Deviation value for color matching.
};

#endif // BAKED_BLOCK_H