#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <memory>
#include "convert_frame.h"
#include "../objects/blocks_db.h"
#include "../objects/baked_block.h"


cv::Mat downscale_image(const std::string& image_path, size_t scale_factor) {
    cv::Mat img = cv::imread(image_path, cv::IMREAD_UNCHANGED);
#if STEP_DEBUG
    cv::imwrite("steps/step0_input.png", img);
#endif // STEP_DEBUG
    [[unlikely]] if (img.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        std::cerr << "Path: " << image_path << std::endl;
        return cv::Mat();
    }

    size_t scaled_height = std::floor(img.rows / scale_factor);
    size_t scaled_width = std::floor(img.cols / scale_factor);

    cv::Size scaled_size(scaled_width, scaled_height);
    cv::Mat scaled_img;

    cv::resize(img, scaled_img, scaled_size, 0, 0, cv::INTER_LANCZOS4);
    return scaled_img;
}


cv::Mat convert_image_to_minecraft_blocks(const cv::Mat& img, const std::string &mode) {
    int pxl_red, pxl_green, pxl_blue, pxl_alpha, current_deviation, min_deviation;
    size_t closest_index = 100e6;

    cv::Mat img_rgba;

    BlocksDB* bdb = BlocksDB::getInstance();

    cv::cvtColor(img, img_rgba, cv::COLOR_RGB2RGBA);
    #if STEP_DEBUG
        cv::imwrite("steps/step2_COLOR_RGB2RGBA.png", img_rgba);
#endif // STEP_DEBUG

    cv::Mat output(img_rgba.rows * 16, img_rgba.cols * 16, CV_8UC4, cv::Scalar(0, 0, 0, 0));

    for (int y = 0; y < img_rgba.rows; ++y) {
        for (int x = 0; x < img_rgba.cols; ++x) {
            // Initialize the closest item and key
            closest_index = 100e6;

            cv::Vec4b pixel_rgb = img_rgba.at<cv::Vec4b>(y, x);
            pxl_blue = pixel_rgb[0];
            pxl_green = pixel_rgb[1];
            pxl_red = pixel_rgb[2];
            
            pxl_alpha = pixel_rgb[3];
            if (mode == "lamp") {
                int avg_color = std::floor((pxl_red + pxl_green + pxl_blue) / 3);
                std::string texture_name = avg_color > 127 ? "redstone_lamp_on.png" : "redstone_lamp.png";
                
                std::stringstream texture_path;
                texture_path << "assets/minecraft/textures/block/" << texture_name;
                cv::Mat block_texture = cv::imread(texture_path.str(), cv::IMREAD_UNCHANGED);

                [[unlikely]] if (block_texture.empty()) {
                    std::cerr << "Could not open or find the texture image!" << std::endl;
                    std::cerr << "Path: " << texture_path.str() << std::endl;
                    continue;
                }

                // Paste the block texture onto the output image
                if (block_texture.channels() < output.channels()) {
                    cv::cvtColor(block_texture, block_texture, cv::COLOR_BGR2BGRA);
                }

                block_texture.copyTo(output(cv::Rect(x * 16, y * 16, 16, 16)));
#if STEP_DEBUG
                std::stringstream step3_path;
                step3_path << "steps/step3/step3_y" << y << "_x" << x << ".png";
                cv::imwrite(step3_path.str(), output);
#endif // STEP_DEBUG
                continue;
            }

            if (pxl_alpha <= 70) continue;


            for (int i = 0; i < bdb->size(); ++i) {
                BakedBlock& block = bdb->at(i);

                current_deviation = std::abs(pxl_red - block.red) + std::abs(pxl_green - block.green) + std::abs(pxl_blue - block.blue);
                if (closest_index == 100e6 || current_deviation < min_deviation) {
                    closest_index = i;
                    min_deviation = current_deviation;
                }
            }

            // Get the closest block and its texture
            BakedBlock &closest_baked_block = bdb->at(closest_index);
            std::stringstream texture_path;
            texture_path << "assets/minecraft/textures/block/" << closest_baked_block.texture_name;
            cv::Mat block_texture = cv::imread(texture_path.str(), cv::IMREAD_UNCHANGED);

            [[unlikely]] if (block_texture.empty()) {
                std::cerr << "Could not open or find the texture image!" << std::endl;
                std::cerr << "Path: " << texture_path.str() << std::endl;
                continue;
            }

            // Paste the block texture onto the output image
            if (block_texture.channels() < output.channels()) {
                cv::cvtColor(block_texture, block_texture, cv::COLOR_BGR2BGRA);
            }

            block_texture.copyTo(output(cv::Rect(x * 16, y * 16, 16, 16)));
#if STEP_DEBUG
            std::stringstream step3_path;
            step3_path << "steps/step3/step3_y" << y << "_x" << x << ".png";
            cv::imwrite(step3_path.str(), output);
#endif // STEP_DEBUG
        }
    }

    return output;
}
