#include <opencv2/opencv.hpp>
#include "convertors/convert_frame.h"


int main(int argc, char** argv) {
    /// Parse arguments
    std::string image_path = argv[1];
    std::string output_path = argv[2];
    std::string mode = argv[3];

    size_t scale = (argc > 4) ? std::stoi(argv[4]) : 1;
    size_t scale_factor = std::floor(16 / scale);

    /// Downscale image
    cv::Mat scaled_img;
    if (scale_factor > 1) {
        scaled_img = downscale_image(image_path, scale_factor);
#if STEP_DEBUG
        cv::imwrite("steps/step1a_downscaled.png", scaled_img);
#endif // STEP_DEBUG
    } else {
        scaled_img = cv::imread(image_path, cv::IMREAD_UNCHANGED);
#if STEP_DEBUG
        cv::imwrite("steps/step1b_not_downscaled.png", scaled_img);
#endif // STEP_DEBUG
    }

    /// Convert image to minecraft blocks
    if (!scaled_img.empty()) {
        cv::Mat minecraft_img = convert_image_to_minecraft_blocks(scaled_img, mode);
#if STEP_DEBUG
        cv::imwrite("steps/step3_converted.png", minecraft_img);
#endif // STEP_DEBUG
        cv::imwrite(output_path, minecraft_img);
    }
    return 0;
}
