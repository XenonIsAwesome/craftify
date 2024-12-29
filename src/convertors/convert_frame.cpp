#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <memory>
#include <vector>

#include "convert_frame.h"
#include "objects/blocks_db.h"
#include "objects/baked_block.h"


cv::Mat downscale_image(const std::string& image_path, int scale_factor = 16) {
    cv::Mat img = cv::imread(image_path, cv::IMREAD_UNCHANGED);
    [[unlikely]] if (img.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return cv::Mat();
    }

    size_t scaled_height = img.rows / scale_factor;
    size_t scaled_width = img.cols / scale_factor;

    cv::Size scaled_size(scaled_width, scaled_height);
    cv::Mat scaled_img;

    cv::resize(img, scaled_img, new_size, 0, 0, cv::INTER_LANCZOS4);
    return scaled_img;
}


cv::Mat convert_image_to_minecraft_blocks(const cv::Mat& img) {
    cv::Mat img_rgba;
    cv::cvtColor(img, img_rgba, cv::COLOR_BGR2RGBA);

    BlocksDB& bdb = BlocksDB::getInstance();

    cv::Mat output(img.rows * 16, img.cols * 16, CV_8UC4, cv::Scalar(0, 0, 0, 0));

    for (int y = 0; y < img.rows; ++y) {
        for (int x = 0; x < img.cols; ++x) {
            // Initialize the closest item and key
            std::shared_ptr<BakedBlock> closest_item = nullptr;
            int closest_key = 0;
            for (int i = 0; i < bdb.size(); ++i) {
                BakedBlock& block = bdb[i];
                cv::Vec4b pixel_rgba = img_rgba.at<cv::Vec4b>(y, x);
                uint8_t r = pixel[0];
                uint8_t g = pixel[1];
                uint8_t b = pixel[2];
                uint8_t a = pixel[3];

                if (a <= 70) continue;

                dev = abs(r - item.red) + abs(g - item.green) + abs(b - item.blue)
                if (closest_item == nullptr || dev < closest_item->deviation) {
                    closest_item = std::make_shared<BakedBlock>(block);
                    closest_key = i;
                }

            }
            // Your logic to find the closest item
            // Update the output with the closest_item's image data
        }
    }

    return output;
}
